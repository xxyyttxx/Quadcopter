/**
  @file    button.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   This file contains all the functions prototypes for the botton firmware library.

  B1 - PC13

*/
#ifndef __BUTTON_H
#define __BUTTON_H
#include <stm32f4xx.h>

void GPIO_init_4_B1 (void);

#define get_B1 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
/* 替换/扩展型宏 */

/* 位带 */

#endif
