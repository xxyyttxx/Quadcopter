/**
  @file    motor-PWM.h
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20171111
  @brief   a header file declares the interface of motor control.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __motor_PWM_H
#define __motor_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <inttypes.h>
#include <stdint.h>


/* Exported constants --------------------------------------------------------*/
#define motor_pwm_arr (2500-1)
#define motor_pwm_max 2000 // 不能给满 要留余量
#define motor_pwm_min 1000 // 电调和接收机都是这样的



/* Exported macro ------------------------------------------------------------*/
#define motor_pwm_1 (*(int32_t*)&TIM1->CCR1)
#define motor_pwm_2 (*(int32_t*)&TIM1->CCR2)
#define motor_pwm_3 (*(int32_t*)&TIM1->CCR3)
#define motor_pwm_4 (*(int32_t*)&TIM1->CCR4)


/* Exported functions ------------------------------------------------------- */
/**
  * @brief  keep CCR in [0, ARR).
  */
inline uint32_t motor_pwm_range (int32_t x) { // boom warning!! (x)<0
	return ((x)>motor_pwm_max ? motor_pwm_max : (x)<motor_pwm_min ? motor_pwm_min : (x)) ;
}

/**
  * @brief  init TIM1_CH[1:4] & GPIOA[8:11](AF01).
  */
void motor_pwm_init (void);

#ifdef __cplusplus
}
#endif


#endif /* __motor_PWM_H */
