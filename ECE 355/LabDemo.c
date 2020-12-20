// ----------------------------------------------------------------------------
// School: University of Victoria, Canada.
// Course: ECE 355 "Microprocessor-Based Systems".
//
// See "system/include/cmsis/stm32f0xx.h" for register/bit definitions.
// See "system/src/cmsis/vectors_stm32f0xx.c" for handler declarations.
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/* Clock prescaler for TIM2 timer: no prescaling */
#define myTIM2_PRESCALER ((uint16_t)0x0000)
/* Maximum possible setting for overflow */
#define myTIM2_PERIOD ((uint32_t)0xFFFFFFFF)

void myGPIOA_Init(void);
void myTIM2_Init(void);
void myEXTI_Init(void);


// Declare/initialize your global variables here...
// NOTE: You'll need at least one global variable
// (say, timerTriggered = 0 or 1) to indicate
// whether TIM2 has started counting or not.
int isFirstEdge = 1;

int main(int argc, char* argv[])
{
	trace_printf("This is Part 2 of Introductory Lab...\n");
	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	myGPIOA_Init();		/* Initialize I/O port PA */
	myTIM2_Init();		/* Initialize timer TIM2 */
	myEXTI_Init();		/* Initialize EXTI */

	while (1)
	{
		// Nothing is going on here...
	}

	return 0;

}


void myGPIOA_Init()
{
	/* Enable clock for GPIOA peripheral */
	// Relevant register: RCC->AHBENR
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	/* Configure PA2 as input */
	// Relevant register: GPIOA->MODER
	GPIOA->MODER &= ~(GPIO_MODER_MODER2);
	/* Ensure no pull-up/pull-down for PA */
	// Relevant register: GPIOA->PUPDR
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2);
	//GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4);
}


void myTIM2_Init()
{
	/* Enable clock for TIM2 peripheral */
	// Relevant register: RCC->APB1ENR
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	/* Configure TIM2: buffer auto-reload, count up, stop on overflow,
	 * enable update events, interrupt on overflow only */
	// Relevant register: TIM2->CR1
	TIM2->CR1 |= ((uint16_t)0x008C);
	/* Set clock prescaler value */
	TIM2->PSC |= myTIM2_PRESCALER;
	/* Set auto-reloaded delay */
	TIM2->ARR |= myTIM2_PERIOD;
	/* Update timer registers */
	// Relevant register: TIM2->EGR
	TIM2->EGR |= ((uint16_t)0x0001);
	/* Assign TIM2 interrupt priority = 0 in NVIC */
	// Relevant register: NVIC->IP[3], or use NVIC_SetPriority
	NVIC_SetPriority(TIM2_IRQn, 0);
	/* Enable TIM2 interrupts in NVIC */
	// Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
	NVIC_EnableIRQ(TIM2_IRQn);
	/* Enable update interrupt generation */
	// Relevant register: TIM2->DIER
	TIM2->DIER |= TIM_DIER_UIE;
}


void myEXTI_Init()
{
	/* Map EXTI2 line to PA2 */
	// Relevant register: SYSCFG->EXTICR[0]
	SYSCFG->EXTICR[0] &= 0x0000F0FF;
	/* EXTI2 line interrupts: set rising-edge trigger */
	// Relevant register: EXTI->RTSR
	EXTI->RTSR |= EXTI_RTSR_TR2;
	/* Unmask interrupts from EXTI2 line */
	// Relevant register: EXTI->IMR
	EXTI->IMR |= EXTI_IMR_MR2;
	/* Assign EXTI2 interrupt priority = 0 in NVIC */
	// Relevant register: NVIC->IP[2], or use NVIC_SetPriority
	/* Enable EXTI2 interrupts in NVIC */
	// Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
	NVIC_SetPriority(EXTI2_3_IRQn, 0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
}


/* This handler is declared in system/src/cmsis/vectors_stm32f0xx.c */
void TIM2_IRQHandler()
{
	/* Check if update interrupt flag is indeed set */
	if ((TIM2->SR & TIM_SR_UIF) != 0)
	{
		trace_printf("\n*** Overflow! ***\n");
		TIM2->SR &= ~(TIM_SR_UIF); /* Clear update interrupt flag */
		TIM2 -> CR1 |= TIM_CR1_CEN; /* Restart stopped timer */
	}
}


/* This handler is declared in system/src/cmsis/vectors_stm32f0xx.c */
void EXTI2_3_IRQHandler()
{
	// Declare/initialize your local variables here...
	//trace_printf("here");
	/* Check if EXTI2 interrupt pending flag is indeed set */
	if ((EXTI->PR & EXTI_PR_PR2) != 0)
	{
		//
		// 1. If this is the first edge:
		//	- Clear count register (TIM2->CNT).
		//	- Start timer (TIM2->CR1).
		//    Else (this is the second edge):
		//	- Stop timer (TIM2->CR1).
		//	- Read out count register (TIM2->CNT).
		//	- Calculate signal period and frequency.
		//	- Print calculated values to the console.
		//	  NOTE: Function trace_printf does not work
		//	  with floating-point numbers: you must use
		//	  "unsigned int" type to print your signal
		//	  period and frequency.
		//
		// 2. Clear EXTI2 interrupt pending flag (EXTI->PR).
		// NOTE: A pending register (PR) bit is cleared
		// by writing 1 to it.
		// Stop timer
		TIM2->CR1 &= ~(TIM_CR1_CEN);
		if(isFirstEdge == 1){
			// Clear count reg
			TIM2->CNT = 0x00000000;
			TIM2->CR1 |= TIM_CR1_CEN;
			isFirstEdge = 0;
		}else{
			// Mask EXTI1 interrupts
			EXTI->IMR &= ~(EXTI_IMR_MR2);
			isFirstEdge = 1;
			unsigned int timerPulse = TIM2->CNT;
			double freq = ((double)SystemCoreClock)/((double)timerPulse);
			trace_printf("Calculated frequency: %4.f hz\n",freq);
			trace_printf("Calculated frequency: %4.4f hz\n",1/freq);
			// Unmask EXTI1 interrupts
			EXTI->IMR |= EXTI_IMR_MR2;
		}
		// Clear interrupt flag
		EXTI->PR |= EXTI_PR_PR2;
	}
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
