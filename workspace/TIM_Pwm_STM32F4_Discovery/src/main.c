/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "functions.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

int main (void)
{
	GPIO_Configure ();
	TIMER_Configure ();

	GPIO_SetBits (GPIOD, GPIO_Pin_12);
	GPIO_ResetBits (GPIOD, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	while (1)
	{
	}
}
