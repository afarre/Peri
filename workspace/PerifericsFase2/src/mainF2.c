/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "delay.h"
#include "clock.h"
#include "tm_stm32f4_disco.h"

int showGreenLED = 1;
int frequencia = 0;

void ADC_Config(void){
    // Enable clock for ADC1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // Init GPIOB for ADC input
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Init ADC1
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStruct.ADC_NbrOfConversion = 300;
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADC1, &ADC_InitStruct);

}

uint16_t ADC_Read(void){
    // Start ADC conversion
    ADC_SoftwareStartConv(ADC1);
    // Wait until conversion is finish
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}

void takeSamples(){
	for (int i = 0; i < 300; i++){
		int adcData = ADC_Read();
	}
}

void GPIO_Configure (void){
    GPIO_InitTypeDef GPIO_InitDef;

	//Activem el clock per els pins G
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	//Inicialitzem leds:
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitDef);
	GPIO_SetBits(GPIOG, GPIO_Pin_13);

}

void BUTTON_Configure(void){
	//Enable clock for GPOIA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	//Pin 0
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	//Mode output
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	//Output type push-pull
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//With pull down resistor
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	//100MHz pin speed
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pin on GPIOA port
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void TIM2_Init(){
    // Enable clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// TIM2 initialization overflow every 500ms
	// TIM2 by default has clock of 84MHz
	// Here, we must set value of prescaler and period,
	// so update event is 0.5Hz or 500ms
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) *
	// (TIM_Period + 1))
	// Update Event (Hz) = 84MHz / ((4199 + 1) * (9999 + 1)) = 2 Hz
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 3599;
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	// TIM2 initialize
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	// Enable TIM2 interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// Nested vectored interrupt settings
	// TIM2 interrupt is most important (PreemptionPriority and
	// SubPriority = 0)
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}


int main(void){
	TIM2_Init();
	GPIO_Configure();
	BUTTON_Configure();

	/* Set clock for GPIOD ------------------------------------------ */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Set alternate function of GPIOD pin 12 as PWM outputs
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	// GPIOD pin 12 as outputs
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);


	/* -------------------------------------------------------------- */
	GPIO_ResetBits(GPIOG, GPIO_Pin_14);
	int count = 0;
    while (1){
		// Delay initialization
		//DELAY_Init();

		if (count >= 500000){
			GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
			count = 0;
		}else{
			count+=10;
		}


    	// Button pressed
    	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
    		// Start TIM2
			TIM_Cmd(TIM2, ENABLE);
			do{
				if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
					// Stop TIM2
					TIM_Cmd(TIM2, DISABLE);

					//BOTO APRETAT: FER COSES
					GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
					//Reset TIM2 value
					TIM2->CNT = 0;
				}
			}while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
    	}else{
    		// Stop TIM2
			TIM_Cmd(TIM2, DISABLE);
    	}
   	}
}

void TIM2_IRQHandler(){
    // Checks whether the TIM2 interrupt has occurred or not

    if (TIM_GetITStatus(TIM2, TIM_IT_Update)){


    	GPIO_SetBits(GPIOG, GPIO_Pin_14);
    	GPIO_ResetBits(GPIOG, GPIO_Pin_13);
    	showGreenLED = 0;


    	ADC_Cmd(ADC1, ENABLE);
		// Select input channel for ADC1
		// ADC1 channel 9 is on PB1
		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_84Cycles);
		takeSamples();

        // Clears the TIM2 interrupt pending bit
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
