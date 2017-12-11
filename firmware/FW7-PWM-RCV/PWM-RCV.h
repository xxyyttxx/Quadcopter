/**
  @file    PWM-RCV.h
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20171113
  @brief   获取接收机PWM的占空比。
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_RCV_H
#define __PWM_RCV_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdint.h>


/* Exported variable ---------------------------------------------------------*/
extern uint16_t u16Rcvr_ch1;
extern uint16_t u16Rcvr_ch2;
extern uint16_t u16Rcvr_ch3;
extern uint16_t u16Rcvr_ch4;

/* Exported functions ------------------------------------------------------- */
/**
  * @brief  init TIM & GPIO
	|==========|==============|======|
	|   GPIO   |    F411RE    | AFxx |
	|==========|==============|======|
	| PC[6:9]  | TIM3_CH[1:4] | AF02 |
	| PA[6:7]  | TIM3_CH[1:2] | AF02 |
	|==========|==============|======|
  */
void RCV_IC_init (void);
void TIM3_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __PWM_RCV_H */
