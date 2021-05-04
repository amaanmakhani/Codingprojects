/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include "stm32f4_discovery.h"
/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"
#include "../Libraries/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h"
#include "../Libraries/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_adc.h"
/*-----------------------------------------------------------*/
static void prvSetupHardware( void );

// Struct and enum used for queue communication
typedef enum type {green, amber, red, flow} type;
typedef struct display {
    type reqType;
    uint16_t flow;
} display;

// Queues, timers, and traffic array
xQueueHandle ADCQueue;
xQueueHandle requestQueue;
xTimerHandle lightTimer;
xTimerHandle trafficTimer;
int traffic[19];

/*-----------------------------------------------------------*/
void initGPIOandADC(void)
{
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

    // Initialize GPIO for the shift register
	GPIO_InitTypeDef ShiftReg_GPIO_Struct;
    // Using pins 6, 7, and 8
	ShiftReg_GPIO_Struct.GPIO_Pin = 0b111000000;
	ShiftReg_GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT;
	ShiftReg_GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	ShiftReg_GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    // An arbitrary speed was chosen
	ShiftReg_GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC , &ShiftReg_GPIO_Struct);

    // Initialize GPIO for the potentiometer
	GPIO_InitTypeDef Potentiometer_GPIO_Struct;
    // Using pin 3
	Potentiometer_GPIO_Struct.GPIO_Pin = 0b1000;
	Potentiometer_GPIO_Struct.GPIO_Mode = GPIO_Mode_AN;
	Potentiometer_GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &Potentiometer_GPIO_Struct);

	//Initialize ADC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitTypeDef ADC_Struct;
	ADC_Struct.ADC_ContinuousConvMode = DISABLE;
	ADC_Struct.ADC_DataAlign = ADC_DataAlign_Left;
	ADC_Struct.ADC_Resolution = ADC_Resolution_8b;
	ADC_Struct.ADC_ScanConvMode = DISABLE;
	ADC_Struct.ADC_ExternalTrigConv = DISABLE;
	ADC_Init(ADC1, &ADC_Struct);
	ADC_Cmd(ADC1, ENABLE );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13 , 1, ADC_SampleTime_56Cycles);

    // Initialize the lights so that only the green light is on at the start
    GPIO_ResetBits(GPIOC, 0);
    GPIO_ResetBits(GPIOC, 1);
	GPIO_SetBits(GPIOC, 4);
}

/*-----------------------------------------------------------*/
void turnOnLED()
{
	// Parallel shift registers used
	// Clock is reset, data is set, clock is then set, and cleared
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
}

/*-----------------------------------------------------------*/
void turnOffLED()
{
	// Parallel shift registers used
	// Clock is reset, data is set, clock is then set, and cleared
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
}

/*-----------------------------------------------------------*/
void sendToShiftRegister()
{
	// Used to output traffic array
    for (int i = 18; i >= 0 ; i--){
        if(traffic[i] == 1){
            turnOnLED();
        } else {
            turnOffLED();
        }
    }
}

/*-----------------------------------------------------------*/
void shiftArrayByOne (int canAdvance)
{
    if(canAdvance == 1){
		// Green light, all traffic moves up one spot
        for (int i = 17; i >= 0; i--){
            traffic[i + 1] = traffic[i];
        }
        traffic[0] = 0;
    } else {
		// Traffic before the light moves up one if the next spot is free
        for (int i = 6; i >= 0; i--){
            if(traffic[i + 1] != 1){
                traffic[i + 1] =  traffic[i];
                traffic[i] = 0;
            }
        }
		// Traffic in or after the intersection moves up one spot
        for (int i = 17; i >= 8; i--){
        	traffic[i + 1] = traffic[i];
        }
        traffic[8] = 0;
        traffic[0] = 0;
    }
}

/*
The traffic flow that enters the intersection is set by a potentiometer.
This task reads the value of the potentiometer at an appropriate interval and sends its value to other tasks.
-----------------------------------------------------------*/
static void flowAdjustmentTask( void *pvParameters )
{
	uint16_t adc_value;
    display temp;
    temp.reqType = flow;
	while(1)
	{
		ADC_SoftwareStartConv(ADC1);
		// Wait for the conversion to finish
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
		// Get the ADC value
		adc_value = ADC_GetConversionValue(ADC1);
		printf("ADC Value: %d\n", adc_value);
        temp.flow = adc_value;
		// Ensure most up to date ADC value is used
        xQueueSendToFront(ADCQueue, &temp,  pdMS_TO_TICKS(500));
        vTaskDelay(pdMS_TO_TICKS(500));
	}
}

/*-----------------------------------------------------------*/
int getDistance(){
	// Distance from the start to the next car
	for(int i = 0; i < 18; i++){
		if(traffic[i] == 1){
			return i;
		}
	}
}

/*
This task randomly generates new traffic with a rate that is proportional to the potentiometer’s value
this value is received from the Traffic Flow Adjustment Task.
The generated traffic is then sent to another task so that it can be displayed on the road.
-----------------------------------------------------------*/
void trafficGeneratorTask( xTimerHandle lightTimer )
{
    display temp;
    xQueueReceive(ADCQueue, &temp, pdMS_TO_TICKS(500));
    if(getDistance() >= (6 - (int)(temp.flow/10000))){
		// New traffic is needed
        temp.flow = 1;
    } else {
		// No new traffic is needed
    	temp.flow = 0;
    }
	// Send a display request for the traffic
    xQueueSend(requestQueue, &temp, pdMS_TO_TICKS(500));
    xTimerStart(trafficTimer, 0);
}

/*This task controls the timing of the traffic lights and outputs it's current state.
The timing of the lights is affected by the load of the traffic which is received from the Traffic Flow Adjustment Task.
-----------------------------------------------------------*/
void trafficLightTask( xTimerHandle lightTimer )
{
	display request;
	xQueueReceive(ADCQueue, &request, pdMS_TO_TICKS(500));
	int temp = (int)pvTimerGetTimerID(lightTimer);
	int p1 = 0;
	int p2 = 0;
	// Calculate the timer period based on the flow
	// If the traffic flow is high the green light should be twice as long as the red light
	// If the traffic flow is low the green light should be half as long as the red light
	if(request.flow > 30000){
		p2 = 750 * (int)(request.flow/10000);
		p1 = 1500 * (int)(request.flow/10000);
	} else {
		p1 = (int)(3000 + 750* (request.flow/10000));
		p2 = (int)(4000 + 1500 * (request.flow/10000));
	}

	// Set new timer period and change the id based on the current lights status
    if(temp == 0){
    	request.reqType = green;
    	vTimerSetTimerID( lightTimer, (void *) 1);
    	xTimerChangePeriod( lightTimer, pdMS_TO_TICKS(p1), 100);
    } else if (temp == 1) {
    	request.reqType = amber;
    	vTimerSetTimerID( lightTimer, (void *) 2);
    	xTimerChangePeriod( lightTimer, pdMS_TO_TICKS(1500), 100);
    } else if (temp == 2) {
    	request.reqType = red;
    	vTimerSetTimerID( lightTimer, (void *) 0);
    	xTimerChangePeriod( lightTimer, pdMS_TO_TICKS(p2), 100);
    }
    request.flow = 0;
	// Send a display request for the light
    xQueueSend(requestQueue, &request, pdMS_TO_TICKS(500));
    xTimerStart(lightTimer, 0);
}

/* This task controls all LEDs in the system and is responsible for visualizing all vehicle traffic and the traffic lights.
It receives information from the Traffic Generator Task as well as the Traffic Light State Task and controls the system’s LEDs accordingly.
This task also refreshes the car LEDs at a certain interval to emulate the flow of the traffic
-----------------------------------------------------------*/
static void systemDisplayTask( void *pvParameters )
{
    display temp;
	// Traffic cannot advance if it is not in or beyond the intersection on a red or amber light
	// A canAdvance with value 1 indicates it is a green light
    int canAdvance = 1;
    while(1)
	{
        if(xQueueReceive(requestQueue, &temp, pdMS_TO_TICKS(500))){
			if(temp.reqType == green){
				// Turn of red and turn on green
				GPIO_ResetBits(GPIOC, 1);
				GPIO_SetBits(GPIOC, 4);
				canAdvance = 1;
			} else if (temp.reqType == amber) {
				// Turn of green and turn on amber
				GPIO_ResetBits(GPIOC, 4);
				GPIO_SetBits(GPIOC, 2);
				canAdvance = 0;
			} else if (temp.reqType == red) {
				// Turn of amber and on red
				GPIO_ResetBits(GPIOC, 2);
				GPIO_SetBits(GPIOC, 1);
				canAdvance = 0;
			} else {
				// Advance traffic
                shiftArrayByOne(canAdvance);
				// Add traffic if needed
                if(temp.flow == 1){
                	traffic[0] = 1;
				}
				// Display traffic
				sendToShiftRegister();
			}
        } else {
        	vTaskDelay(250);
        }
	}
}

/*-----------------------------------------------------------*/
int main(void)
{	
	// Initialize the ADC, GPIO, and the hardware
	prvSetupHardware();
	initGPIOandADC();
	// Create the queues
    ADCQueue = xQueueCreate(100, sizeof(display));
    requestQueue = xQueueCreate(100, sizeof(display));
	// Add to the registry, for the benefit of kernel aware debugging.
	vQueueAddToRegistry(ADCQueue, "ADCQueue" );
	vQueueAddToRegistry(requestQueue, "requestQueue" );
	// Create a ADC task and a Display task
	xTaskCreate(flowAdjustmentTask, "ADC_task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(systemDisplayTask, "systemDisplayTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	// Create a timer to generate the traffic and generate new traffic periodically
    lightTimer = xTimerCreate("trafficLightTask", pdMS_TO_TICKS(1500), pdFALSE, (void *) 0, trafficLightTask);
    xTimerStart(lightTimer, 0);
    trafficTimer = xTimerCreate("trafficGeneratorTask", pdMS_TO_TICKS(1000), pdFALSE, (void *) 3, trafficGeneratorTask);
    xTimerStart(trafficTimer, 0);
	// Start the scheduler
	vTaskStartScheduler();
	return 0;
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* The malloc failed hook is enabled by setting
	configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

	Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software 
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  pxCurrentTCB can be
	inspected in the debugger if the task name passed into this function is
	corrupt. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
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
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Ensure all priority bits are assigned as preemption priority bits.
	http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	NVIC_SetPriorityGrouping( 0 );

	/* TODO: Setup the clocks, etc. here, if they were not configured before
	main() was called. */
}