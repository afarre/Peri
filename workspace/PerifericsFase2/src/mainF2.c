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

int fastLED = 0;
int count = 0;
int freq = 0;
uint16_t totalMostres[300];
volatile uint32_t status = 0;
volatile uint32_t i;
__IO uint16_t ADC3ConvertedValue = 0;
__IO uint32_t ADC3ConvertedVoltage = 0;

#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)
#define ARRAYSIZE 300

//initialize destination arrays
uint16_t destination[ARRAYSIZE];

typedef enum {
		TM_DAC1, /*!< DAC channel 1 */
		TM_DAC2  /*!< DAC channel 2 */
	} TM_DAC_Channel_t;

void DAC_Inicialitza() {
	DAC_InitTypeDef DAC_InitStruct;
	uint16_t GPIO_Pin;

	/* Select pin A4 for DAC1 */
	GPIO_Pin = GPIO_PIN_4;

	/* Initialize proper GPIO pin */
	TM_GPIO_Init(GPIOA, GPIO_Pin, TM_GPIO_Mode_AN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);

	/* Enable DAC clock */
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;

	/* Set DAC options */
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	/* Init and enable DAC1 */
	DAC_Init(DAC_Channel_1, &DAC_InitStruct);
	/* Enable DAC channel 1 */
	DAC->CR |= DAC_CR_EN1;
}


void initDAC(){
	DAC_Inicialitza();

	while(1){
		for (int i = 0; i < ARRAYSIZE; i++){
			DAC->DHR12R1 = destination[i];
		}
	}
}


 void startMemoryToMemoryTransfer(){

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	DMA_InitTypeDef  DMA_InitStructure;

	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&totalMostres;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&destination;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
	DMA_InitStructure.DMA_BufferSize = ARRAYSIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC8;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC8;
	DMA_Init(DMA2_Stream3, &DMA_InitStructure);

	DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);

	  // Enable DMA
	  DMA_Cmd(DMA2_Stream3, ENABLE);
	  /*

	 for (int i = 0; i < 300; i++){
		 destination[i] = totalMostres[i];
	 }
	 */

}


void DMA2_Stream3_IRQHandler(void) {

	if(DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3)) {
		DMA_Cmd(DMA2_Stream3, DISABLE);
		DMA_ClearITPendingBit(DMA2_Stream3, DMA_FLAG_TCIF3);
		initDAC();
	}
}

/**
  * @brief  ADC3 channel07 with DMA configuration
  * @param  None
  * @retval None
  */
void ADC3_CH7_DMA_Config(void){
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  /* Configure ADC3 Channel7 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC3 regular channel7 configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_3Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(ADC3, ENABLE);

  /* Enable ADC3 */
  ADC_Cmd(ADC3, ENABLE);
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
	//SOURCE: https://www.rapitasystems.com/blog/setting-up-a-free-running-timer-on-the-stm-32-discovery-f4-board
	TIM_TimeBaseInitTypeDef SetupTimer;
	/* Enable timer 2, using the Reset and Clock Control register */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	SetupTimer.TIM_Prescaler = 0x0000;
	SetupTimer.TIM_CounterMode = TIM_CounterMode_Up;
	SetupTimer.TIM_Period = 0xFFFFFFFF;
	SetupTimer.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &SetupTimer);
	TIM_Cmd(TIM2, ENABLE); /* start counting by enabling CEN in CR1 */
}

void reconfigTimer2(){
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
		TIM_TimeBaseInitStruct.TIM_Prescaler = 1;
		TIM_TimeBaseInitStruct.TIM_Period = 47;
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

	//ADC
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init (GPIOA, &GPIO_InitStructure);
}

void LEDToggle(){
	if (count >= 400000){
		GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
		count = 0;
	}else{
		count+=10;
	}
}

int main(void){
	//TIM4 config source: http://embeddedsystemengineering.blogspot.com/2015/08/stm32f4-discovery-tutorial-10-pwm.html
	//TIM4 config source: https://stm32f4-discovery.net/2014/05/stm32f4-stm32f429-discovery-pwm-tutorial/

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOA, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM2_Init();
	GPIO_Configure();
	BUTTON_Configure();
	ADC3_CH7_DMA_Config();
	//ADC3_CH7_DMA_Config source: https://github.com/Dima-Meln/stm32-cmake/blob/master/stm32f4/libs/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Project/STM32F4xx_StdPeriph_Examples/ADC/ADC3_DMA/main.c


	//	ADC_Config();
	//ADC config source 1: http://electronicatk.blogspot.com/2016/01/adc-stm32f4-discovery.html
	//ADC config source 2: http://embeddedsystemengineering.blogspot.com/2015/07/stm32f4-discovery-tutorial-7-adc.html

	/* -------------------------------------------------------------- */
	GPIO_ResetBits(GPIOG, GPIO_Pin_14);
	int LED = 1;
    while (1){
		// Delay initialization
		//DELAY_Init();

    	// Button pressed
    	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
    		// Save TIM2 start value
    		unsigned long countStart = TIM_GetCounter(TIM2);

    		do{
    			//do LED toggle while button pressed
    			if(LED){
    				LEDToggle();
    			}
			}while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));

    		//USer stopped pressing button
			// Save TIM2 end value
			unsigned long countStop = TIM_GetCounter(TIM2);

			if((countStop - countStart) < 60000000){
				//button pressed for less than 2.5seconds
				freq++;
				if (freq >= 3){
					freq = 0;
				}
			}else{
				//button pressed for more than 2.5seconds
				GPIO_ResetBits(GPIOG, GPIO_Pin_13);
				//GPIO_SetBits(GPIOG, GPIO_Pin_14);
				LED = 0;
				reconfigTimer2();
				TIM_Cmd(TIM2, ENABLE);
			}
    	}
    	//do LED toggle while button unpressed
    	if(LED){
			LEDToggle();
		}
   	}
}

int compt = 0;
int mostres = 0;
void TIM2_IRQHandler(){
    // Checks whether the TIM2 interrupt has occurred or not
    if (TIM_GetITStatus(TIM2, TIM_IT_Update)){
//    	GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
//    	int adcData = 0;

    	if(mostres < 300){
    		if (freq == 0){
    			//freq == 0 -> una mostra cada 10us
    			//adcData = ADC_Read();
    			/* Start ADC3 Software Conversion */
    			ADC_SoftwareStartConv(ADC3);
    			totalMostres[mostres] = ADC3ConvertedValue;
    			mostres++;
    		}else if (freq == 1){
    			//freq == 1 -> una mostra cada 100us
    			compt++;
    			if (compt == 10){
    				//adcData = ADC_Read();
    				ADC_SoftwareStartConv(ADC3);
    				compt = 0;
    				totalMostres[mostres] = ADC3ConvertedValue;
    				mostres++;
    			}
    		}else if (freq == 2){
    			//freq == 2 -> una mostra cada 1ms
    			compt++;
				if (compt == 100){
					//adcData = ADC_Read();
					ADC_SoftwareStartConv(ADC3);
					compt = 0;
					totalMostres[mostres] = ADC3ConvertedValue;
					mostres++;
				}
    		}

    	}else{
    		ADC_DMACmd(ADC3, DISABLE);
    		TIM_Cmd(TIM2, DISABLE);
    		GPIO_SetBits(GPIOG, GPIO_Pin_14);
    		startMemoryToMemoryTransfer();
    		GPIO_ResetBits(GPIOG, GPIO_Pin_14);
        }
        // Clears the TIM2 interrupt pending bit
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
