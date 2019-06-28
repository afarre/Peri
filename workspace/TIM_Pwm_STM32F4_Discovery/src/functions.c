/*
 * functions.c
 *
 *  Created on: 25/6/2015
 *
 */

#include "functions.h"

void GPIO_Configure (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig (GPIOD, GPIO_PinSource15 , GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init (GPIOD, &GPIO_InitStructure);
}

void TIMER_Configure (void)
{
	TIM_TimeBaseInitTypeDef TIMER_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIMER_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM7, ENABLE);

	TIMER_TimeBaseInitStructure.TIM_Prescaler = 8400 - 1;
	TIMER_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER_TimeBaseInitStructure.TIM_Period = 10000 - 1;
	TIMER_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMER_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit (TIM7, &TIMER_TimeBaseInitStructure);

	TIM_ITConfig (TIM7, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init (&NVIC_InitStructure);

	TIM_Cmd (TIM7, ENABLE);

	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);

	TIMER_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIMER_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER_TimeBaseInitStructure.TIM_Period = 8400 - 1;
	TIMER_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMER_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit (TIM4, &TIMER_TimeBaseInitStructure);

	TIMER_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIMER_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIMER_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIMER_OCInitStructure.TIM_Pulse = 1 - 1;
	TIM_OC4Init (TIM4, &TIMER_OCInitStructure);
	TIM_OC4PreloadConfig (TIM4, TIM_OCPreload_Enable);

	TIM_Cmd (TIM4, ENABLE);
}
