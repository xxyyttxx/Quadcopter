/**
  @file    motor-PWM.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20171111
  @brief   a source file implement the motor controlor.
*/


/* Includes ------------------------------------------------------------------*/
// #include <assert.h>
#include "motor-PWM.h"

/* Exported functions ---------------------------------------------------------*/
extern uint32_t motor_pwm_range (int32_t x);

/**
  * @brief  init TIM1_CH[1:4] & GPIOA[8:11](AF01).
  */
void motor_pwm_init (void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	{
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);
		GPIO_InitTypeDef r1 = {GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11, GPIO_Mode_AF, GPIO_High_Speed, GPIO_OType_PP, GPIO_PuPd_NOPULL};
		GPIO_Init(GPIOA, &r1);
	}

	{ /* 可能TIM1 有不同的默认值，但是结构体的默认值一定是0呀.. */
		TIM_TimeBaseInitTypeDef r1 = {
			.TIM_Prescaler = 99,
			// .TIM_CounterMode = TIM_CounterMode_Up,
			.TIM_Period = motor_pwm_arr // 1M/motor_pwm_arr
			// , .TIM_ClockDivision = TIM_CKD_DIV1
		};
		TIM_TimeBaseInit(TIM1, &r1);
	}

	{
		TIM_OCInitTypeDef r1 = {
			.TIM_OCMode = TIM_OCMode_PWM1,
			.TIM_OutputState = TIM_OutputState_Enable,
			.TIM_OutputNState = TIM_OutputNState_Enable // ?
			// .TIM_Pulse = 0,
			// .TIM_OCPolarity = TIM_OCPolarity_High,
			// .TIM_OCNPolarity = TIM_OCPolarity_High
		};
		TIM_OC1Init(TIM1, &r1);
		TIM_OC2Init(TIM1, &r1);
		TIM_OC3Init(TIM1, &r1);
		TIM_OC4Init(TIM1, &r1);
	}

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	// 注释掉下面这两行，电调两秒一哔（表示没有信号输入）
	// 但是之前LD2-PWM没有这两行却能正确的输出PWM。
	// 留下疑问，日后再解决。
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);

	TIM_Cmd(TIM1, ENABLE);
}
