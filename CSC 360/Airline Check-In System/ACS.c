#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>    // Thread functionality
#include <sys/types.h>  // Thread functionality types
#include <unistd.h>     // Used for usleep
#include <sys/time.h>   // Used for time retrieval

// User input constraints
#define MAX_INPUT_SIZE 9999
// Clerk constraint
#define NUMCLERKS 5
// Used for customer creation
struct customerInformation{
    int id;
    // 1 Business class
    // 0 Economy class
    int class;
    int arrivalTime;
    int serviceTime;
};
typedef struct customerInformation customerInformation;

// Mutexes
pthread_mutex_t timeLock;
pthread_mutex_t queueLock;
pthread_mutex_t clerkAvailable;
pthread_mutex_t nextClerk;
pthread_mutex_t clerkOccupied[NUMCLERKS];
// Condition variables
pthread_cond_t serving;
pthread_cond_t queueChanged;
pthread_cond_t customerServed[NUMCLERKS];
// Thread pools
pthread_t clerks[NUMCLERKS];

// Global variables
// Data that needs mutex protection
struct timeval programBeginTime;
double waitTimes[2];
int businessQueueLength = 0;
int economyQueueLength = 0;
int businessQueueIndex = 0;
int economyQueueIndex  = 0;
int businessQueue[MAX_INPUT_SIZE];
int economyQueue[MAX_INPUT_SIZE];
int toBeServed = 0;
int serverID = 0;
// Holds clerk status, used in conjuction with conditional variables
// This was to battle spontaneous wakeups
int clerkBusy[NUMCLERKS];
// Data arrays used to store class count, used for end of program statistics
int classCount[2];

// Summarize customer service time
void printStats(){
    printf("The average waiting time for all customers in the system is: %.3f seconds.\n", (waitTimes[0] + waitTimes[1])/(classCount[0] + classCount[1]));
    printf("The average waiting time for all business-class customers is: %.3f seconds.\n", waitTimes[1]/classCount[1]);
    printf("The average waiting time for all economy-class customers is: %.3f seconds.\n", waitTimes[0]/classCount[0]);
}

void* clerk(void* id) {
    int clerkID = *((int*)id);
    while(1)
    {
        /******** Clerk critical section starts *******/
        // Only one clerk can grab the next customer
        pthread_mutex_lock(&nextClerk);
        // Set clerk to the next nextClerk
        serverID = clerkID;

        /******** Queue access critical section starts *******/
        // Wait for someone to serve
        pthread_mutex_lock(&queueLock);
        while(businessQueueLength - businessQueueIndex == 0 && economyQueueLength - economyQueueIndex == 0)
        {
            toBeServed = -1;
            pthread_cond_wait(&queueChanged, &queueLock);
        }
        // Choose business queue if available otherwise choose economy queue
        if(businessQueueLength - businessQueueIndex != 0)
        {
            toBeServed = businessQueue[businessQueueIndex];
            businessQueueIndex++;
        } else {
            toBeServed = economyQueue[economyQueueIndex];
            economyQueueIndex++;
        }
        pthread_mutex_unlock(&queueLock);
        /******** Queue access critical section ends *******/

        pthread_mutex_unlock(&clerkOccupied[clerkID - 1]);
        clerkBusy[clerkID - 1] = 1;
        // Signal customers that a clerk is serving a customer
        pthread_cond_broadcast(&serving);
        // Allow customer threads waiting to have already got back to call pthread_cond_wait
        usleep(10);
        // Allow another clerk to serve a customer
        pthread_mutex_unlock(&nextClerk);
        /******** Clerk critical section ends *******/

        // Wait for customer to finish
        while (clerkBusy[clerkID - 1] != 0)
        {
            pthread_cond_wait(&customerServed[clerkID - 1], &clerkOccupied[clerkID - 1]);
        }
    }
    pthread_exit(NULL);
}

void* customer(void* details) {
    struct timeval beginWaitTime;
    struct timeval currentTime;
    customerInformation* info = (customerInformation*) details;
    usleep(info->arrivalTime * 100000);
    printf("Customer %d arrives.\n", info->id);
    
    /******** Queue access critical section starts *******/
    // Add the customer to a queue, increase the length, and document the event
    switch(info->class){
        case 0:
            pthread_mutex_lock(&queueLock);
            economyQueueLength++;
            printf("Customer %d enters the economy class queue, that has a length of %d.\n", info->id, economyQueueLength - economyQueueIndex);
            economyQueue[economyQueueLength - 1] = info->id;
            pthread_mutex_unlock(&queueLock);
            break;
        case 1:
            pthread_mutex_lock(&queueLock);
            businessQueueLength++;
            printf("Customer %d enters the business class queue, that has a length of %d.\n", info->id, businessQueueLength - businessQueueIndex);
            businessQueue[businessQueueLength - 1] = info->id;
            pthread_mutex_unlock(&queueLock);
            break;
        default:
            printf("Customer %d: invalid class.\n", info->id);
            pthread_exit(NULL);
    }
    /******** Queue access critical section ends *******/

    // Keep track of waiting time and signal clerk
    gettimeofday(&beginWaitTime, NULL);
    // Clerk is available let clerk know line is not empty
    if(toBeServed == -1){
        pthread_cond_signal(&queueChanged);
    }

    /******** Clerk available critical section starts *******/
    // While my customer number isn't being served wait for a clerk to be available
    while (toBeServed != info->id)
    {
        pthread_cond_wait(&serving, &clerkAvailable);
    }
    int servedBy = serverID;

    /******** Clerk occupied critical section starts *******/
    pthread_mutex_lock(&clerkOccupied[servedBy - 1]);
    pthread_mutex_unlock(&clerkAvailable);
    /******** Clerk available critical section ends *******/

    /******** Wait time access critical section starts *******/
    gettimeofday(&currentTime, NULL);
    printf("Clerk %d starts serving customer %d, start time %.2f.\n", servedBy, info->id, (double)(currentTime.tv_usec - programBeginTime.tv_usec) / 1000000 + (double)(currentTime.tv_sec - programBeginTime.tv_sec));
    pthread_mutex_lock(&timeLock);
    waitTimes[info->class] += ((double)(currentTime.tv_usec - beginWaitTime.tv_usec) / 1000000 + (double)(currentTime.tv_sec - beginWaitTime.tv_sec));
    pthread_mutex_unlock(&timeLock);
    /******** Wait time access critical section ends *******/

    // Customer gets served for service time
    usleep(info->serviceTime * 100000);
    clerkBusy[servedBy - 1] = 0;
    pthread_mutex_unlock(&clerkOccupied[servedBy - 1]);
    /******** Clerk occupied critical section ends *******/

    // Notify clerk that customer has been served
    pthread_cond_signal(&customerServed[servedBy - 1]);
    gettimeofday(&currentTime, NULL);
    printf("Clerk %d finishes serving customer %d, end time %.2f.\n", servedBy, info->id, (double)(currentTime.tv_usec - programBeginTime.tv_usec) / 1000000 + (double)(currentTime.tv_sec - programBeginTime.tv_sec));
    pthread_exit(NULL);
}

void checkForError(int retValue, char* operation){
    if (retValue != 0)
    {
        printf("%s failed\n", operation);
        exit(-1);
    }
}

void createConditionalVariables(){
    for(int i = 0; i < NUMCLERKS; ++i)
    {
        checkForError(pthread_cond_init(&customerServed[i], NULL), "Conditional variable initialization");
    }
    checkForError(pthread_cond_init(&serving, NULL), "Conditional variable initialization");
    checkForError(pthread_cond_init(&queueChanged, NULL), "Conditional variable initialization");
}

void destroyConditionalVariables(){
    for(int i = 0; i < NUMCLERKS; ++i)
    {
        checkForError(pthread_cond_destroy(&customerServed[i]), "Conditional variable destruction");
    }
    checkForError(pthread_cond_destroy(&serving), "Conditional variable destruction");
    checkForError(pthread_cond_destroy(&queueChanged), "Conditional variable destruction");

}

void createMutexes(){
    checkForError(pthread_mutex_init(&timeLock, NULL), "Mutex initialization");
    checkForError(pthread_mutex_init(&queueLock, NULL), "Mutex initialization");
    checkForError(pthread_mutex_init(&clerkAvailable, NULL), "Mutex initialization");
    checkForError(pthread_mutex_init(&nextClerk, NULL), "Mutex initialization");
    for(int i = 0; i < NUMCLERKS; ++i)
    {
        checkForError(pthread_mutex_init(&clerkOccupied[i], NULL), "Mutex initialization");
    }
}

void createClerks(){
    for(int i = 0; i < NUMCLERKS; ++i)
    {
        clerkBusy[i] = i + 1;
        checkForError(pthread_create(&clerks[i], NULL, clerk, (void*)&clerkBusy[i]), "Thread creation");
    }
}

void destroyClerks(){
    for(int i = 0; i < NUMCLERKS; ++i)
    {
        checkForError(pthread_cancel(clerks[i]), "Thread creation");
    }
}

int main(int argc, char *argv[]) {
    char line[MAX_INPUT_SIZE];
    if(argc != 2)
    {
        printf("Too many or too few arguments\n");
        exit(-1);
    }
    FILE *fptr = fopen(argv[1],"r");
    if (fptr == NULL)
    {
        printf("Error: Failed to open file %s.\n", argv[1]);
        exit(-1);
    }
    else 
    {
        // Initialize global variables
        waitTimes[0] = 0;
        waitTimes[1] = 0;
        classCount[0] = 0;
        classCount[1] = 0;
        gettimeofday(&programBeginTime, NULL);
        createMutexes();
        // Get the line containing the cutomer count
        fgets(line, MAX_INPUT_SIZE, fptr);
        char *token = strtok(line, ":,\n");
        int customerCount = atoi(token);
        // Create variables for using customer count
        pthread_t customers[customerCount];
        customerInformation info[customerCount];
        // Create customers using information from the file
        for(int i = 0; i < customerCount; i++)
        {
            // Get id
            fgets(line, MAX_INPUT_SIZE, fptr);
            token = strtok(line, ":,\n");
            token != NULL ? info[i].id = atoi(token) : exit(-1);
            // Get class
            token = strtok(NULL, ":,\n");
            token != NULL ? info[i].class = atoi(token) : exit(-1);
            info[i].class == 1 ? classCount[1]++ : classCount[0]++;
            // Get arrival time
            token = strtok(NULL, ":,\n");
            token != NULL ? info[i].arrivalTime = atoi(token) : exit(-1);
            // Get service time
            token = strtok(NULL, ":,\n");
            token != NULL ? info[i].serviceTime = atoi(token) : exit(-1);
            // Create customer thread using obtained information
            checkForError(pthread_create(&customers[i], NULL, customer, (void*)&info[i]), "Thread creation"); 
        }
        createClerks();
        // Wait for all customers to be served
        for(int i = 0; i < customerCount; i++)
        {
            pthread_join(customers[i],NULL);
        }
        printStats();
        // Cleanup resources
        destroyClerks();
        destroyConditionalVariables();
    }
    fclose(fptr);
    exit(0);
}