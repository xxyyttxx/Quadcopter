/**
  @file    main.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   define main entry.
*/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "LD2pwm.h"
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  */
int main(void)
{
	delayinit();
	GPIO_init_4_LD2_PWM();
  while (1) {
		for (int i = 1; i<99; i++) 
		{
			pwm_LD2 = i;
			delay(20);
		}
		for (int i = 99; i>1; i--)
		{
			pwm_LD2 = i;
			delay(10);
		}
	}
}
