/**
  @file    btn_exti.h
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170724
  @brief   a header file about the button triggers an external interrupt.
           B1 - PC13 - EXTI13
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BTN_EXTI_H
#define __BTN_EXTI_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"   
#include "ucos_ii.h"
#include "stm32f4xx_exti.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GPIO_init_4_BTN_EXTI (void);
void EXTI15_10_IRQHandler (void);

#ifdef __cplusplus
}
#endif

#endif /* __BTN_EXTI_H */
