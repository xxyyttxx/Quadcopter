/**
  @file    button.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   This file contains all the functions prototypes for the LED firmware library.

  LD2 - PA5 - TIM2_CH1

*/
#ifndef __LED_H
#define __LED_H
#include <stm32f4xx.h>

void GPIO_init_4_LD2 (void);

#define set_LD2(s) GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(s))
/* 扩展型宏 */


/* 位带 */

#endif
