/**
  @file    LD2pwm.h
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   interface of pwm output on LD2.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LD2PWM_H
#define __LD2PWM_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifndef pwm_arr
	#define pwm_arr 100
#endif
#define pwm_LD2 (TIM2->CCR1)
#define pwm_range(x) ((x)>=pwm_arr?pwm_arr-1:(x)<0?0:x) /* danger tri.. */
/* Exported functions ------------------------------------------------------- */
void GPIO_init_4_LD2_PWM (void);

#ifdef __cplusplus
}
#endif

#endif /* __LD2PWM_H */
