/**
  @file    button.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   This file provides firmware functions to user LED on nucleo64.

  LD2 - PA5 - TIM2_CH1
  
*/
#include <stm32f4xx.h>
#include <LED.h>

void GPIO_init_4_LD2 (void)
{
	GPIO_InitTypeDef r1 = {GPIO_Pin_5, GPIO_Mode_OUT, GPIO_High_Speed, GPIO_OType_PP, GPIO_PuPd_NOPULL};
	GPIO_InitTypeDef r2 = {GPIO_Pin_5, GPIO_Mode_OUT, GPIO_High_Speed, GPIO_OType_OD, GPIO_PuPd_UP};
	(void)r1; // PP
	(void)r2; // OD
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &r1);
}
