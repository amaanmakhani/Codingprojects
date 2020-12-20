// ----------------------------------------------------------------------------
// School: University of Victoria, Canada.
// Course: ECE 355 "Microprocessor-Based Systems".
// See "system/include/cmsis/stm32f0xx.h" for register/bit definitions.
// See "system/src/cmsis/vectors_stm32f0xx.c" for handler declarations.
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "stm32f0xx.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// Clock prescaler for TIM2 timer: no prescaling
#define myTIM2_PRESCALER ((uint16_t)0x0000)
// Maximum possible setting for overflow
#define myTIM2_PERIOD ((uint32_t)0xFFFFFFFF)

void myGPIOA_Init(void);
void myGPIOB_Init(void);
void myADC_Init(void);
void myDAC_Init(void);
void myTIM2_Init(void);
void myTIM3_Init(void);
void wait(int duration);
void myEXTI_Init(void);
int ReadADC(void);
void WriteDAC(uint32_t digitalval);
void myLCD_Init(void);
void write_to_lcd(uint16_t data);
void printValues(int resistance, int frequency);

// Global variables
int isFirstEdge = 0;
float freq = 0;

int main(int argc, char* argv[]) {
    myGPIOA_Init();     /* Initialize I/O port PA   */
    myGPIOB_Init();     /* Initialize GPIO B        */
    myTIM2_Init();      /* Initialize timer TIM2    */
    myTIM3_Init();      /* Initialize timer TIM2    */
    myEXTI_Init();      /* Initialize EXTI          */
    myADC_Init();       /* Initialize ADC           */
    myDAC_Init();       /* Initialize DAC           */
    myLCD_Init();       /* initialize LCD           */
    while (1)
    {
        int adcval = ReadADC();                                     /* Save ADC value to relay to DAC                   */
        WriteDAC(adcval);                                           /* Relay value from potentiometer to optocoupler    */
        float potResistance = ((3.3 * (DAC->DOR1))/4095)/0.00066;   /* Save potentiometer resistance                    */
        printValues(potResistance, freq);                           /* Update LCD with frequency and Resistance values  */
        wait(100000);                                               /* Delay screen update                              */
    }
    return 0;
}

void myADC_Init(){
    ADC1->CR = ((uint16_t)0x0);             /* Clear ADC                    */
    GPIOC->MODER |= ((uint16_t)0x20);       /* PC1 to analog                */
    RCC->APB2ENR |= ((uint16_t)0x0200);     /* Turn on ADC Clock            */
    ADC1->CHSELR |= ((uint16_t)0x0800);     /* Enable Channel 11            */
    ADC1->CFGR2 |= ((uint16_t)0x0000);      /* Enable synchronous mode      */
    ADC1->CR |= ((uint16_t)0x80000000);     /* Calibrate ADC                */
    while((ADC1->CR == 0x80000000));        /* Wait for ADC to calibrate    */
    ADC1->CR |= ((uint16_t)0x1);            /* Enable ADC                   */
}

int ReadADC () {
    ADC1->CR |= ((uint16_t)0x5);            /* Turn on ADC                  */
    while((ADC1->ISR & 0x4) == 1);          /* Wait for end of conversion   */
    int ADCvalue = ADC1->DR;                /* Save adc value               */
    ADC1->ISR &= ~(0x4);                    /* Clear end of conversion flag */
    return ADCvalue;
}

void myDAC_Init() {
    DAC->CR = ((uint16_t)0x0);              /* Clear DAC                */
    GPIOA->MODER &= ~((uint32_t)0x300);     /* Set PA4 as Analog out    */
    RCC->APB1ENR |= ((uint32_t)0x20000000); /* Enable DAC Clock         */
    DAC->CR = ((uint16_t)0x01);             /* Enable DAC               */
}

void WriteDAC(uint32_t digitalval) {
    // Data holding register automatically transfers to output register
    DAC->DHR12R1 = (unsigned int)digitalval;
}

void myGPIOA_Init(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;      /* Enable clock for GPIOA peripheral    */
    GPIOA->MODER &= ~(GPIO_MODER_MODER1);   /* Configure PA1 as input               */
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR1);   /* Ensure no pull-up/pull-down for PA1  */
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4);   /* Ensure no pull-up/pull-down for PA4  */
}

void myTIM2_Init(){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;     /* Enable clock for TIM2 peripheral             */
    TIM2->CR1 |= ((uint16_t)0x008C);        /* Configure TIM2                               */
    TIM2->PSC |= myTIM2_PRESCALER;          /* Set clock prescaler value                    */
    TIM2->ARR |= myTIM2_PERIOD;             /* Set auto-reloaded delay                      */
    TIM2->EGR |= ((uint16_t)0x0001);        /* Update timer registers                       */
    NVIC_SetPriority(TIM2_IRQn, 0);         /* Assign TIM2 interrupt priority = 0 in NVIC   */
    NVIC_EnableIRQ(TIM2_IRQn);              /* Enable TIM2 interrupts in NVIC               */
    TIM2->DIER |= TIM_DIER_UIE;             /* Enable update interrupt generation           */
}

void myTIM3_Init(){
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;     /* Enable Clock for TIM3 Peripheral */
    TIM3->PSC = 480;                        /* Set prescaler                    */
    TIM3->CR1 = ((uint16_t)0x8C);           /* Update & auto reload             */
    TIM3->ARR = 10;                         /* Auto reload default value        */
    TIM3->EGR = ((uint16_t)0x0001);         /* Update timer registers           */
}

void wait(int duration) {
    TIM3->SR = 0x0;                         /* Reset status register                                */
    TIM3->CNT = 1;                          /* Clear count                                          */
    TIM3->ARR = duration;                   /* Auto reload with duration requested - timeout value  */
    TIM3->EGR = 0x01;
    TIM3->CR1 |= 0x01;                      /* Enable clock                                         */
    while((TIM3->SR & 0x1) == 0);           /* Wait until count reached                             */
    TIM3->CR1 = ((uint16_t)0x8C);           /* Reset to value in initialization                     */
    TIM3->SR = 0x0;                         /* Clear status register                                */
}

void myEXTI_Init()
{
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI1_PA;	/* Map EXTI1 line to PA1                            */
	EXTI->RTSR |= EXTI_RTSR_TR1;	                /* EXTI1 line interrupts: set rising-edge trigger   */
	EXTI->IMR |= EXTI_IMR_MR1;	                    /* Unmask interrupts from EXTI1 line                */
	NVIC_SetPriority(EXTI0_1_IRQn, 0);	            /* Assign EXTI1 interrupt priority = 0 in NVIC      */
	NVIC_EnableIRQ(EXTI0_1_IRQn);	                /* Enable EXTI1 interrupts in NVIC                  */
}

void TIM2_IRQHandler(){
    
    if ((TIM2->SR & TIM_SR_UIF) != 0){              /* Check if update interrupt flag is indeed set */
        TIM2->SR = ((uint16_t)0x00);                /* Clear update interrupt flag                  */
        TIM2->CR1 = ((uint16_t)0x01);               /* Restart stopped timer                        */
    }
}

void EXTI0_1_IRQHandler()
{
	if ((EXTI->PR & EXTI_PR_PR1) != 0){                             /* Check if EXTI2 interrupt pending flag    */
		TIM2->CR1 &= ~(TIM_CR1_CEN);
		if(isFirstEdge == 1){
			TIM2->CNT = 0x00000000;                                 /* Clear count reg                          */
			TIM2->CR1 |= TIM_CR1_CEN;
			isFirstEdge = 0;
		}else{
			EXTI->IMR &= ~(EXTI_IMR_MR1);                           /* Mask EXTI1 interrupts                    */
			isFirstEdge = 1;
			unsigned int timerPulse = TIM2->CNT;
			freq = ((double)SystemCoreClock)/((double)timerPulse);
			EXTI->IMR |= EXTI_IMR_MR1;                              /* Unmask EXTI1 interrupts                  */
		}
		EXTI->PR |= EXTI_PR_PR1;                                    /* Clear interrupt flag                     */
	}
}

void myGPIOB_Init() {
    // Enable GPIOB clock
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 
    // Enable pins for output
    GPIOB->MODER = 0x0;
    GPIOB->MODER |= GPIO_MODER_MODER4_0;
    GPIOB->MODER |= GPIO_MODER_MODER5_0;
    GPIOB->MODER |= GPIO_MODER_MODER6_0;
    GPIOB->MODER |= GPIO_MODER_MODER8_0;
    GPIOB->MODER |= GPIO_MODER_MODER9_0;
    GPIOB->MODER |= GPIO_MODER_MODER10_0;
    GPIOB->MODER |= GPIO_MODER_MODER11_0;
    GPIOB->MODER |= GPIO_MODER_MODER12_0;
    GPIOB->MODER |= GPIO_MODER_MODER13_0;
    GPIOB->MODER |= GPIO_MODER_MODER14_0;
    GPIOB->MODER |= GPIO_MODER_MODER15_0;
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR4);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR5);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR6);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR8);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR9);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR10);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR11);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR12);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR13);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR14);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR15);
}

void myLCD_Init() {
    write_to_lcd(0x3800);   /* Function set     */
    write_to_lcd(0x0F00);   /* Display on/off   */
    write_to_lcd(0x0600);   /* Entry mode       */
    write_to_lcd(0x0100);   /* Clear display    */
}

void write_to_lcd(uint16_t data) {
	GPIOB-> ODR = 0x0000;                       /* Clear ODR register                                   */
    GPIOB-> ODR |= data;                        /* Write data                                           */
    GPIOB-> ODR |= GPIO_ODR_4;                  /* Make PB[4] = 1 (assert “Enable”)                     */
    while((GPIOB-> IDR & GPIO_ODR_7) == 0);     /* Wait for PB[7] to become 1 (“Done” to be asserted)   */
    GPIOB-> ODR  &= ~(GPIO_ODR_4);              /* Make PB[4] = 0 (deassert “Enable”)                   */
    while((GPIOB-> IDR & GPIO_ODR_7) != 0);     /* Wait for PB[7] to become 0 (“Done” to be deasserted) */
}

void printValues(int resistance, int frequency) {
    // Isolate each digit
    int digit4 = (resistance % 10);
    int digit3 = (((resistance - digit4) % 100) / 10);
    int digit2 = (((resistance - digit4 - (10*digit3)) % 1000) / 100);
    int digit1 = (resistance/1000);
    write_to_lcd(0x8000);                               /* Set DDRAM address (first line)   */
    write_to_lcd(0x5220);                               /* R                                */
    write_to_lcd(0x3A20);                               /* :                                */
    write_to_lcd((((0x30 | digit1) << 8) | 0x20));      /* #                                */
    write_to_lcd((((0x30 | digit2) << 8) | 0x20));      /* #                                */
    write_to_lcd((((0x30 | digit3) << 8) | 0x20));      /* #                                */
    write_to_lcd((((0x30 | digit4) << 8) | 0x20));      /* #                                */
    write_to_lcd(0x4F20);                               /* O                                */
    write_to_lcd(0x4820);                               /* H                                */
    // Isolate each digit
    digit4 = (frequency % 10);
    digit3 = (((frequency - digit4) % 100) / 10);
    digit2 = (((frequency - digit4 - (10*digit3)) % 1000) / 100);
    digit1 = (frequency/1000);
    write_to_lcd(0xC000);                               /* Set DDRAM address (second line)  */
    write_to_lcd(0x4620);                               /* F                                */
    write_to_lcd(0x3A20);                               /* :                                */
    write_to_lcd((((0x30 | digit1) << 8) | 0x20));      /* #                                */
    write_to_lcd((((0x30 | digit2) << 8) | 0x20));      /* #                                */
    write_to_lcd((((0x30 | digit3) << 8) | 0x20));      /* #                                */
    write_to_lcd((((0x30 | digit4) << 8) | 0x20));      /* #                                */
    write_to_lcd(0x4820);                               /* H                                */
    write_to_lcd(0x7A20);                               /* Z                                */
}
#pragma GCC diagnostic pop
// ----------------------------------------------------------------------------