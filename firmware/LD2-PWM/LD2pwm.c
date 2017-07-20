/**
  @file    LD2pwm.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   interface of pwm output on LD2.
*/

/* Includes ------------------------------------------------------------------*/
#include "LD2pwm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void GPIO_init_4_LD2_PWM (void)
{
	
	{
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
		GPIO_InitTypeDef r1 = {GPIO_Pin_5, GPIO_Mode_AF, GPIO_High_Speed, GPIO_OType_PP, GPIO_PuPd_NOPULL};
		GPIO_Init(GPIOA, &r1);
	}
	
	{
		TIM_TimeBaseInitTypeDef r1 = {
			.TIM_CounterMode = TIM_CounterMode_Up,
			.TIM_ClockDivision = TIM_CKD_DIV1,
			.TIM_Prescaler = 0,
			.TIM_Period = pwm_arr //100M->1M
		};
		TIM_TimeBaseInit(TIM2, &r1);
	}
	
	{
		TIM_OCInitTypeDef r1 = {
			r1.TIM_OCMode = TIM_OCMode_PWM1,
			r1.TIM_OCPolarity = TIM_OCPolarity_High,
			r1.TIM_OutputState = TIM_OutputState_Enable
		};
		TIM_OC1Init(TIM2, &r1);
	}
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	
	TIM_Cmd(TIM2, ENABLE);
	/*
  -@- If the corresponding interrupt or DMA request are needed, the user should:
        (+@) Enable the NVIC (or the DMA) to use the TIM interrupts (or DMA requests). 
        (+@) Enable the corresponding interrupt (or DMA request) using the function 
             TIM_ITConfig(TIMx, TIM_IT_CCx) (or TIM_DMA_Cmd(TIMx, TIM_DMA_CCx))  
	*/
}

