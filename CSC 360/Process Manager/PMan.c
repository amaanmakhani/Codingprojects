#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Includes for process control
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
// Add custom linked list library
#include "linkedList.h"
// User input constants
#define MAX_INPUT_SIZE 9999

void manageProcessStatus(pid_t pid, node** list, char* signal){
    if(findElement(list, pid, 0))
    {
        int retValue = 0;
        // Check what signal should be sent to existing process
        if(strncmp(signal, "TERM", 4) == 0)
        {
            retValue = kill(pid, SIGTERM);
        }
        else if(strncmp(signal, "STOP", 4) == 0)
        {
            retValue = kill(pid, SIGSTOP);
        }
        else if (strncmp(signal, "CONT", 4) == 0)
        {
            retValue = kill(pid, SIGCONT);
        }
        else
        {
            printf("Error: Invalid signal.\n");
            return;
        }
        if(retValue == -1)
        {
            printf("Error: kill() failed.\n");
        }
    }
    else
    {
        printf("Error: Process %d does not exist.\n", pid);
    }
}

void createProcess(node** list, char** parameters){
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {   
        // Execute program in child process
        if (execvp(parameters[0], parameters) < 0)
        {
            // On failure terminate
            printf("Error: execvp failed.\n");
            exit(-1);
        }
    }
    else if(pid > 0) 
    {
        // Track process
        addTaskToTheList(list, createNode(pid, parameters[0]));
    }
    else 
    {
        printf("Error: Fork failed.\n");
    }
}

void pstat(pid_t pid, node* list){
    node* searchResult = findElement(&list, pid, 0);
    if(searchResult)
    {
        // Variables used for file processing
        char line[MAX_INPUT_SIZE];
        char* data[MAX_INPUT_SIZE];
        char path[1024];
        // Open /proc/[pid]/stat
        sprintf (path, "/proc/%d/stat", pid);
        FILE *fptr = fopen(path,"r");
        if (fptr == NULL)
        {
            printf("Error: Failed to open file %s.\n", path);

        }
        else 
        {
            fgets(line, MAX_INPUT_SIZE, fptr);
            // Parse the line by spaces and save it to be accessed later
            int index = 0;
            char* token;
            token = strtok(line, " ");
            while (token != NULL)
            {
                data[index] = token;
                token = strtok(NULL, " ");
                index++;
            }
            // Print the needed file contents 
            printf("comm: %s\n", data[1]);
            printf("state: %s\n", data[2]);
            // Divide usage cycles by system clock to obtain time
            char *ptr;
            printf("utime: %f\n", (float)strtoul(data[13], &ptr, 10)/(float)sysconf(_SC_CLK_TCK));
            printf("stime: %f\n", (float)strtoul(data[14], &ptr, 10)/(float)sysconf(_SC_CLK_TCK));
            printf("rss: %s\n", data[23]);
            fclose(fptr);
        }

        // Open /proc/[pid]/status
        sprintf(path, "/proc/%d/status", pid);
        fptr = fopen(path,"r");
        if (fptr == NULL)
        {
            printf("\nError: Failed to open file %s.\nPMan: > ", path);

        }
        else 
        {
            // Data is seperated by a newline character
            // Iterate through data and print last two items for context switch information
            int index = 0;
            while (fgets(line, MAX_INPUT_SIZE, fptr) != NULL)
            {
                index++;
                if(index == 54)
                {
                    printf("%s", line);
                }
            }
            printf("%s", line);
            fclose(fptr);
        }
    }
    else
    {
        printf("Error: Process %d does not exist.\n", pid);
    }
}

void checkZombieProcess(node** list){
    int	status;
    while (1)
    {
        // No processes to check
        if(list == NULL)
        {
            return;
        }
        // Allow process status to change prior to checking status
        usleep(1500);
        // Wait for any child process whose process group ID is equal to the absolute value of pid
        // Options used:
        // WCONTINUED: return if a stopped child has been resumed by delivery of SIGCONT
        // WNOHANG: return immediately if no child has exited.
        // WUNTRACED: return if a child has stopped
        pid_t retPid = waitpid(-1, &status, WCONTINUED | WNOHANG | WUNTRACED);
        if (retPid > 0)
        {   
            // Find the pid node and update it's status or remove it from the list
            node* searchResult = findElement(list, retPid, 0);
            if (WIFSTOPPED(status) && searchResult != NULL) 
            {
                printf("Process %d was stopped.\n", retPid);
                searchResult->active = 0;
            }
            else if (WIFCONTINUED(status) && searchResult != NULL) 
            {
                printf("Process %d was started.\n", retPid);
                searchResult->active = 1;
            }
            else if (WIFSIGNALED(status)) 
            {
                printf("Process %d was killed.\n", retPid);
                findElement(list, retPid, 1);		
            }
            else if (WIFEXITED(status)) 
            {
                printf("Process %d terminated.\n", retPid);
                findElement(list, retPid, 1);			
            }
        }
        else 
        {
            break;
        }
    }
}

int main(){
    // Storage variables
    node* listHead = NULL;
    char* userInput = auto_malloc(MAX_INPUT_SIZE);
    char command[8];
    // Variables used during parsing
    pid_t pid;
    char *token;
    // User input flags used to process commands
    int twoArgumentsProvided = 0;
    int validPid = 0;

    while(1){
        // Reset storage variables
        memset(userInput, '\0', MAX_INPUT_SIZE);
        memset(command, '\0', 8);
        // Reset flags
        twoArgumentsProvided = 0;
        validPid = 0;
        // Prompt user for input
        printf("PMan: > ");
        // Read users input
        // Ignores newline character, tabs, and space
        // Returns once a newline character is encountered
        while(scanf(" %[^\n\t]", userInput) == 0);
        checkZombieProcess(&listHead);
        // Copy input so original input exists untokenized
        token = strtok(userInput, " ");
        // Acceptable commands only have a length of 7 so no need to copy more characters
        strncpy(command, token, 7);
        // Parse the remianing parameters
        char* parameters[MAX_INPUT_SIZE];
        // Used to indicate the number of parameters
        parameters[0] = NULL;
        parameters[1] = NULL;
        int index = 0;
        while(token != NULL)
        {
            token = strtok(NULL, " ");
            parameters[index] = token;
            index++;
        }
        // Check if second argument could be possibly a pid
        if(parameters[1] == NULL && parameters[0] != NULL)
        {
            pid = atoi(parameters[0]);
            // Check validity of pid
            // Max pid found using 
            // cat /proc/sys/kernel/pid_max
            // Max pid is 32768
            if( 0 < pid  && pid <= 4194304)
            {
                validPid = 1;
            }
            // Set flag to indicate a parameter was entered
            twoArgumentsProvided = 1;
        }

        // Compare the input to acceptable commands
        if(strncmp(command, "bg", 8) == 0 && parameters[0] != NULL)
        {
            createProcess(&listHead, parameters);            
        }
        else if(strncmp(command, "bglist", 8) == 0)
        {
            if(parameters[0] != NULL){
                printf("PMan: > %s: too many arguments\n", command);
                continue;
            }
            printList(listHead);
        }
        else if(strncmp(command, "bgkill", 8) == 0 && twoArgumentsProvided && validPid)
        {
            manageProcessStatus(pid, &listHead, "TERM");
        }
        else if(strncmp(command, "bgstop", 8) == 0 && twoArgumentsProvided && validPid)
        {
            manageProcessStatus(pid, &listHead, "STOP");
        }
        else if(strncmp(command, "bgstart", 8) == 0 && twoArgumentsProvided && validPid)
        {
            manageProcessStatus(pid, &listHead, "CONT");
        }
        else if(strncmp(command, "pstat", 8) == 0 && twoArgumentsProvided && validPid)
        {
            pstat(pid, listHead);
        }
        else if(strncmp(command, "bg"       , 8) == 0 ||
                strncmp(command, "bgkill"   , 8) == 0 ||
                strncmp(command, "bgstop"   , 8) == 0 ||
                strncmp(command, "bgstart"  , 8) == 0 ||
                strncmp(command, "pstat"    , 8) == 0)
        {   
            // Correct command but command was used incorrectly, alert the user
            if(!twoArgumentsProvided)
            {
                printf("PMan: > %s: too few or too many arguments\n", command);
            }
            else if(!validPid)
            {
                printf("PMan: > %d: invalid pid, pid must be greater than 0 and less than 4194305\n", pid);
            }
        }
        else
        {
            printf("PMan: > %s: command not found\n", userInput);  
        }
        checkZombieProcess(&listHead);
    }
    freeList(listHead);
    free(userInput);
}