/**
  @file    button.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   This file provides firmware functions to user botton on nucleo64.

  B1 - PC13

*/
#include <stm32f4xx.h>
#include <button.h>

void GPIO_init_4_B1 (void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// 默认浮空输入
}
