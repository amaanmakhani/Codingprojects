// Standard includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "stm32f4_discovery.h"
// Kernel includes
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"
#include "../Libraries/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h"

// Controls the test bench execute
// The hyper period value at which the test should be run until
#define TEST_BENCH 1
#define REPORTING_PERIOD 1510

// Multiplier used in delay counter
uint32_t multiplier;

// Timer used for overdue task detection
xTimerHandle overdueDetectionTimer;

// Timers used for monitor task and task generators
xTimerHandle monitorTimer;
xTimerHandle Task1Generator;
xTimerHandle Task2Generator;
xTimerHandle Task3Generator;

// Linked list nodes and task representation data structures
typedef enum task_type {PERIODIC, APERIODIC} task_type;

typedef struct dd_task {
    TaskHandle_t t_handle;
    task_type type;
    uint32_t task_id;
    uint32_t release_time;
    uint32_t absolute_deadline;
    uint32_t completion_time;
} dd_task;

typedef struct node{
    dd_task* task;
    struct node* next;
} node;

// Information needed by the task generator based on the test bench
typedef struct generator_information {
    TaskHandle_t t_handle;
    int cycleNum;
    uint32_t period;
} generator_information;

// Stores information for the each test bench
generator_information taskInformation1;
generator_information taskInformation2;
generator_information taskInformation3;

// DDS queue and request data structure
typedef struct request {
    int reqType;
    dd_task* task;
    uint32_t task_id;
    bool isOverdue;
} request;
xQueueHandle ddsQueue;

// DDS response queue and data structure
typedef struct response {
    node* list;
} response;
xQueueHandle responseQueue;

// Scheduler list heads, only accessible by the scheduler
node* activeListHead = NULL;
node* completedListHead = NULL;
node* overdueListHead = NULL;

// Delay counter initialization
void TM_Delay_Init(void) {
    // Code for the delay counter from
    // https://stm32f4-discovery.net/2014/09/precise-delay-counter/
	RCC_ClocksTypeDef RCC_Clocks;
	/* Get system clocks */
	RCC_GetClocksFreq(&RCC_Clocks);
	/* While loop takes 4 cycles */
	/* For 1 us delay, we need to divide with 4M */
	multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
}

// Initialize GPIOs for LED's
void initGPIO(void){
    // Enable GPIOC clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    // Initialize GPIO for the traffic light
	GPIO_InitTypeDef TrafficLight_GPIO_Struct;
    // Using pins 0, 1, and 2
    // Red, amber, and green respectively
	TrafficLight_GPIO_Struct.GPIO_Pin = 0b111;
	TrafficLight_GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT;
	TrafficLight_GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	TrafficLight_GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC , &TrafficLight_GPIO_Struct);
}

// Get the current absolute time
int getAbsoluteTime(){
    int absoluteTime = xTaskGetTickCount()/portTICK_PERIOD_MS;
    return absoluteTime;
}

// Linked list functions
void *auto_malloc(size_t size){
	// Execute a safe malloc operation
    void *ptr = NULL;
	ptr = malloc(size);
	if (ptr == NULL) {
		printf("Malloc failed");
		exit(1);
	}
	return ptr;
}

void freeList(node* list){
   node* tmp;
   while (list != NULL){
       tmp = list;
       list = list->next;
       free(tmp->task);
       free(tmp);
    }
}

void freeAllLists(){
    freeList(activeListHead);
    freeList(completedListHead);
    freeList(overdueListHead);
}

node* createNode(dd_task* task){
    node* newNode = (node*)auto_malloc(sizeof(node));
    newNode->task = task;
    newNode->next = NULL;
    return newNode;
}

void printList(node* list){
    while (list != NULL) {
        printf("task id: %u, release time: %u, completion time: %u, absolute deadline: %u\n", list->task->task_id, list->task->release_time, list->task->completion_time, list->task->absolute_deadline);
        list = list->next;
    }
}

int getListLength(node* list){
    int length = 0;
    while (list != NULL) {
        length+=1;
        list = list->next;
    }
    return length;
}

bool addToTheSortedList(node** list, dd_task* task){
    task->release_time = getAbsoluteTime();
	node* newNode = createNode(task);
    node* listNode = *list;
    if(*list == NULL){
        *list = newNode;
        return true;
    }
    if(listNode->task->absolute_deadline > task->absolute_deadline){
        newNode->next = listNode;
        *list = newNode;
        return true;
    }
    while (listNode->next != NULL){
        if(listNode->next->task->absolute_deadline > task->absolute_deadline){
            newNode->next = listNode->next;
            listNode->next = newNode;
            return false;
        }
        listNode = listNode->next;
    }
    listNode->next = newNode;
    return false;
}

void addToTheEndOfTheList(node** list, node* newNode){
    newNode->next = NULL;
    node* listNode = *list;
    if(*list == NULL){
        *list = newNode;
        return;
    }
    while (listNode->next != NULL){
        listNode = listNode->next;
    }
    listNode->next = newNode;
}

node* removeNode(node** list, int task_id){
    node* removedNode = NULL;
    if((*list)->task->task_id == task_id){
        removedNode = *list;
        vTaskSuspend((*list)->task->t_handle);
        *list = (*list)->next;
        removedNode->task->completion_time = getAbsoluteTime();
        return removedNode;
    }
    node* listNode = *list;
    while (listNode->next != NULL) {
        if(listNode->next->task->task_id == task_id){
            removedNode = listNode->next;
            vTaskSuspend(removedNode->task->t_handle);
            listNode->next = listNode->next->next;
            removedNode->task->completion_time = getAbsoluteTime();
            return removedNode;
        }
        listNode = listNode->next;
    }
    removedNode->task->completion_time = getAbsoluteTime();
    return removedNode;
}

// Scheduler interface functions which are external to the scheduler
// These interfaces send different requests to the scheduler
void create_dd_task(TaskHandle_t t_handle, task_type type, uint32_t task_id, uint32_t absolute_deadline){
    request newRequest;
    dd_task* newTask = (dd_task*)auto_malloc(sizeof(dd_task));
    newTask->type = type;
    newTask->t_handle = t_handle;
    newTask->task_id = task_id;
    newTask->absolute_deadline = absolute_deadline;
    newRequest.task = newTask;
    newRequest.reqType = 1;
    xQueueSend(ddsQueue, &newRequest, pdMS_TO_TICKS(500));
}

void complete_dd_task(uint32_t task_id, bool isOverdue){
    request newRequest;
	newRequest.reqType = 2;
    newRequest.task_id = task_id;
    newRequest.isOverdue = isOverdue;
    xQueueSend(ddsQueue, &newRequest, pdMS_TO_TICKS(500));
}

node* get_active_dd_task_list(void){
    request newRequest;
    newRequest.reqType = 3;
    response returnMessage;
    xQueueSend(ddsQueue, &newRequest, pdMS_TO_TICKS(500));
    xQueueReceive(responseQueue, &returnMessage, pdMS_TO_TICKS(500));
    return returnMessage.list;
}

node* get_complete_dd_task_list(void){
    request newRequest;
    newRequest.reqType = 4;
    response returnMessage;
    xQueueSend(ddsQueue, &newRequest, pdMS_TO_TICKS(500));
    xQueueReceive(responseQueue, &returnMessage, pdMS_TO_TICKS(500));
    return returnMessage.list;
}

node* get_overdue_dd_task_list(void){
    request newRequest;
    newRequest.reqType = 5;
    response returnMessage;
    xQueueSend(ddsQueue, &newRequest, pdMS_TO_TICKS(500));
    xQueueReceive(responseQueue, &returnMessage, pdMS_TO_TICKS(500));
    return returnMessage.list;
}

// User defined tasks
static void userDefinedTask1(void *pvParameters){
    int executionTime = 0;
    while(1){
        executionTime = (int) pvParameters;
        GPIO_SetBits(GPIOC, 0);
        GPIO_ResetBits(GPIOC, 1);
        GPIO_ResetBits(GPIOC, 4);
        // Code for the delay counter from
        // https://stm32f4-discovery.net/2014/09/precise-delay-counter/
        // 400 used as multiplier instead of 1000, as using 1000 produced a very imprecise delay
        executionTime = 400 * executionTime * multiplier - 10;
        while (executionTime > 0){
            executionTime--;
        }
        complete_dd_task(1, false);
    }
}

static void userDefinedTask2(void *pvParameters){
    int executionTime = 0;
    while(1){
        executionTime = (int) pvParameters;
        GPIO_ResetBits(GPIOC, 0);
        GPIO_SetBits(GPIOC, 1);
        GPIO_ResetBits(GPIOC, 4);
        // Code for the delay counter from
        // https://stm32f4-discovery.net/2014/09/precise-delay-counter/
        // 400 used as multiplier instead of 1000, as using 1000 produced a very imprecise delay
        executionTime = 400 * executionTime * multiplier - 10;
        while (executionTime > 0){
            executionTime--;
        }
        complete_dd_task(2, false);
    }
}

static void userDefinedTask3(void *pvParameters){
    int executionTime = 0;
    while(1){
        executionTime = (int) pvParameters;
        GPIO_ResetBits(GPIOC, 0);
        GPIO_ResetBits(GPIOC, 1);
        GPIO_SetBits(GPIOC, 4);
        // Code for the delay counter from
        // https://stm32f4-discovery.net/2014/09/precise-delay-counter/
        // 400 used as multiplier instead of 1000, as using 1000 produced a very imprecise delay
        executionTime = 400 * executionTime * multiplier - 10;
        while (executionTime > 0){
            executionTime--;
        }
        complete_dd_task(3, false);
    }
}

// Deadline driven task generator
static void ddsTaskGenerator(TimerHandle_t xTimer){
    uint32_t task_id = (uint32_t)pvTimerGetTimerID(xTimer);
    if(task_id == 1){
        taskInformation1.cycleNum += 1;
        create_dd_task(taskInformation1.t_handle, PERIODIC, task_id, taskInformation1.period * taskInformation1.cycleNum);
    } else if(task_id == 2){
        taskInformation2.cycleNum += 1;
        create_dd_task(taskInformation2.t_handle, PERIODIC, task_id, taskInformation2.period * taskInformation2.cycleNum);
    } else if(task_id == 3){
        taskInformation3.cycleNum += 1;
        create_dd_task(taskInformation3.t_handle, PERIODIC, task_id, taskInformation3.period * taskInformation3.cycleNum);
    }
}

// Monitor task
static void monitorTask(void *pvParameters){
    printf("Active list length: %d\n", getListLength(get_active_dd_task_list()));
    printf("Completed list length: %d\n", getListLength(get_complete_dd_task_list()));
    printf("Overdue list length: %d\n\n", getListLength(get_overdue_dd_task_list()));
}

// Cleanup function
void cleanup(){
    printf("Final report\n");
    printf("Active list length: %d\n", getListLength(get_active_dd_task_list()));
    printList(get_active_dd_task_list());
    printf("\nCompleted list length: %d\n", getListLength(get_complete_dd_task_list()));
    printList(get_complete_dd_task_list());
    printf("\nOverdue list length: %d\n", getListLength(get_overdue_dd_task_list()));
    printList(get_overdue_dd_task_list());
    freeAllLists();
    vTaskEndScheduler();
}

// Overdue task detected
void taskIsOverdue(){
    complete_dd_task((int)pvTimerGetTimerID(overdueDetectionTimer), true);
}

// DDS scheduler helper
void startTask(bool *isRunning, int *taskRunning){
    if(activeListHead == NULL){
        return;
    }
    // Create timer to stop overdue tasks
    if(activeListHead->task->absolute_deadline - getAbsoluteTime() > 0){
        if(overdueDetectionTimer == NULL){
            overdueDetectionTimer = xTimerCreate("overdueTaskDetection", pdMS_TO_TICKS (1 + (activeListHead->task->absolute_deadline) - getAbsoluteTime()), pdFALSE, (void*)activeListHead->task->task_id, taskIsOverdue);
        } else {
            vTimerSetTimerID(overdueDetectionTimer, (void*)activeListHead->task->task_id);
            xTimerChangePeriod(overdueDetectionTimer, pdMS_TO_TICKS (1 + (activeListHead->task->absolute_deadline) - getAbsoluteTime()), 0);
        }
        vTaskResume(activeListHead->task->t_handle);
        xTimerReset(overdueDetectionTimer, 0);
        *isRunning = true;
        *taskRunning = activeListHead->task->task_id;
    } else {
        // Deadline is already passed
        // Add to the over due list and start another task if one exists
        addToTheEndOfTheList(&overdueListHead, removeNode(&activeListHead, activeListHead->task->task_id));
        startTask(isRunning, taskRunning);
    }
}

// DDS scheduler task
static void ddsSchedulerTask(void *pvParameters){
    request receivedRequest;
    response returnMessage;
    bool isRunning = false;
    int taskRunning = -1;
	while(1)
	{
        xQueueReceive(ddsQueue, &receivedRequest, pdMS_TO_TICKS(portMAX_DELAY));
        if(receivedRequest.reqType == 1){
            if(taskRunning != receivedRequest.task->task_id){
                vTaskSuspend(receivedRequest.task->t_handle);
            }
            if(addToTheSortedList(&activeListHead, receivedRequest.task)){
                if(activeListHead->next != NULL){
                    vTaskSuspend(activeListHead->next->task->t_handle);
                }
                isRunning = false;
                if(overdueDetectionTimer != NULL){
                    xTimerStop(overdueDetectionTimer, 0);
                }
            }
            if(!isRunning){
                startTask(&isRunning, &taskRunning);
            }
        } else if(receivedRequest.reqType == 2 && !receivedRequest.isOverdue){
            xTimerStop(overdueDetectionTimer, 0);
            addToTheEndOfTheList(&completedListHead, removeNode(&activeListHead, receivedRequest.task_id));
            startTask(&isRunning, &taskRunning);
        } else if(receivedRequest.reqType == 2 && receivedRequest.isOverdue){
            addToTheEndOfTheList(&overdueListHead, removeNode(&activeListHead, receivedRequest.task_id));
            startTask(&isRunning, &taskRunning);
        } else if(receivedRequest.reqType == 3){
            returnMessage.list = activeListHead;
            xQueueSend(responseQueue, &returnMessage, pdMS_TO_TICKS(500));
        } else if(receivedRequest.reqType == 4){
            returnMessage.list = completedListHead;
            xQueueSend(responseQueue, &returnMessage, pdMS_TO_TICKS(500));
        } else if(receivedRequest.reqType == 5){
            returnMessage.list = overdueListHead;
            xQueueSend(responseQueue, &returnMessage, pdMS_TO_TICKS(500));
        }
	}
}

// Main to setup tests and scheduler
int main(){
    initGPIO();
    // Initialize delay counter
	TM_Delay_Init();
    // Create queues
    ddsQueue = xQueueCreate(100, sizeof(request));
    responseQueue = xQueueCreate(100, sizeof(response));
    // Add the queues to the registry, for the benefits while debugging
	vQueueAddToRegistry(ddsQueue, "ddsQueue");
	vQueueAddToRegistry(responseQueue, "responseQueue");
    // Create DDS scheduler
   	xTaskCreate(ddsSchedulerTask, "ddsSchedulerTask", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    // Create monitor F-task
    monitorTimer = xTimerCreate("monitorTask", pdMS_TO_TICKS(500), pdTRUE, 0, monitorTask);
    // Create a timer to end the scheduler after a set duration
    TimerHandle_t reportingPeriod = xTimerCreate("WrapUp", pdMS_TO_TICKS(REPORTING_PERIOD), pdFALSE, 0, cleanup);
    // Execute test bench
    if(TEST_BENCH == 1){
    	generator_information task1 = {NULL, 0, 500};
    	xTaskCreate(userDefinedTask1, "userDefinedTask1", configMINIMAL_STACK_SIZE, (void*)95, 2, &task1.t_handle);
    	taskInformation1 = task1;
        Task1Generator = xTimerCreate("ddsTask1Generator", pdMS_TO_TICKS(500), pdTRUE, (void*)1, ddsTaskGenerator);
        generator_information task2 = {NULL, 0, 500};
        xTaskCreate(userDefinedTask2, "userDefinedTask2", configMINIMAL_STACK_SIZE, (void*)150, 2, &task2.t_handle);
        taskInformation2 = task2;
        Task2Generator = xTimerCreate("ddsTask2Generator", pdMS_TO_TICKS(500), pdTRUE, (void*)2, ddsTaskGenerator);
        generator_information task3 = {NULL, 0, 750};
        xTaskCreate(userDefinedTask3, "userDefinedTask3", configMINIMAL_STACK_SIZE, (void*)250, 2, &task3.t_handle);
        taskInformation3 = task3;
        Task3Generator = xTimerCreate("ddsTask3Generator", pdMS_TO_TICKS(750), pdTRUE, (void*)3, ddsTaskGenerator);
    } else if(TEST_BENCH == 2){
    	generator_information task1 = {NULL, 0, 250};
    	xTaskCreate(userDefinedTask1, "userDefinedTask1", configMINIMAL_STACK_SIZE, (void*)95, 2, &task1.t_handle);
    	taskInformation1 = task1;
        Task1Generator =xTimerCreate("ddsTask1Generator", pdMS_TO_TICKS(250), pdTRUE, (void*)1, ddsTaskGenerator);
        generator_information task2 = {NULL, 0, 500};
        xTaskCreate(userDefinedTask2, "userDefinedTask2", configMINIMAL_STACK_SIZE, (void*)150, 2, &task2.t_handle);
        taskInformation2 = task2;
        Task2Generator = xTimerCreate("ddsTask2Generator", pdMS_TO_TICKS(500), pdTRUE, (void*)2, ddsTaskGenerator);
        generator_information task3 = {NULL, 0, 750};
        xTaskCreate(userDefinedTask3, "userDefinedTask3", configMINIMAL_STACK_SIZE, (void*)250, 2, &task3.t_handle);
        taskInformation3 = task3;
        Task3Generator = xTimerCreate("ddsTask3Generator", pdMS_TO_TICKS(750), pdTRUE, (void*)3, ddsTaskGenerator);
    } else if(TEST_BENCH == 3){
    	generator_information task1 = {NULL, 0, 500};
    	xTaskCreate(userDefinedTask1, "userDefinedTask1", configMINIMAL_STACK_SIZE, (void*)100, 2, &task1.t_handle);
    	taskInformation1 = task1;
        Task1Generator = xTimerCreate("ddsTask1Generator", pdMS_TO_TICKS(500), pdTRUE, (void*)1, ddsTaskGenerator);
        generator_information task2 = {NULL, 0, 500};
        xTaskCreate(userDefinedTask2, "userDefinedTask2", configMINIMAL_STACK_SIZE, (void*)200, 2, &task2.t_handle);
        taskInformation2 = task2;
        Task2Generator = xTimerCreate("ddsTask2Generator", pdMS_TO_TICKS(500), pdTRUE, (void*)2, ddsTaskGenerator);
        generator_information task3 = {NULL, 0, 500};
        xTaskCreate(userDefinedTask3, "userDefinedTask3", configMINIMAL_STACK_SIZE, (void*)200, 2, &task3.t_handle);
        taskInformation3 = task3;
        Task3Generator = xTimerCreate("ddsTask3Generator", pdMS_TO_TICKS(500), pdTRUE, (void*)3, ddsTaskGenerator);
    }
    // Start the initial tasks for time 0
    ddsTaskGenerator(Task1Generator);
    ddsTaskGenerator(Task2Generator);
    ddsTaskGenerator(Task3Generator);
    // Start monitor timer
    //xTimerStart(monitorTimer, 0);
    // Start the task generators
    xTimerStart(Task1Generator, 0);
    xTimerStart(Task2Generator, 0);
    xTimerStart(Task3Generator, 0);
    // Start the reporting period
    xTimerStart(reportingPeriod, 0);
    // Start the scheduler
	vTaskStartScheduler();
}

/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook(void){
	/* The malloc failed hook is enabled by setting
	configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

	Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName){
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  pxCurrentTCB can be
	inspected in the debugger if the task name passed into this function is
	corrupt. */
	for( ;; );
}

void vApplicationIdleHook(void){
    volatile size_t xFreeStackSpace;

	/* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
	FreeRTOSConfig.h.

	This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}

void prvSetupHardware(void){
	/* Ensure all priority bits are assigned as preemption priority bits.
	http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	NVIC_SetPriorityGrouping( 0 );
	/* TODO: Setup the clocks, etc. here, if they were not configured before
	main() was called. */
}
