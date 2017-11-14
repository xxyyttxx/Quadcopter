/**
  @file    PWM-RCV.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20171113
  @brief   获取接收机PWM的占空比。
*/

/* Includes ------------------------------------------------------------------*/
#include "PWM-RCV.h"

/* Private macro ------------------------------------------------------------*/
// var name transfer
#define u16Tim3IC3 u16Rcvr_ch4
#define u16Tim3IC4 u16Rcvr_ch3
#define u16Tim3IC1 u16Rcvr_ch2
#define u16Tim3IC2 u16Rcvr_ch1

/* Exported variables ---------------------------------------------------------*/
uint16_t u16Tim3IC1 = 0;
uint16_t u16Tim3IC2 = 0;
uint16_t u16Tim3IC3 = 0;
uint16_t u16Tim3IC4 = 0;

/* Exported functions ------------------------------------------------------- */
/**
  * @brief  init TIM & GPIO
    |==========|==============|======|
    |   GPIO   |    F411RE    | AFxx |
    |==========|==============|======|
    | PC[8:9]  | TIM3_CH[3:4] | AF02 |
    | PA[6:7]  | TIM3_CH[1:2] | AF02 |
    |==========|==============|======|
  */
void RCV_IC_init (void) {

    /* Configuration PC[8:9]  | TIM3_CH[3:4] | AF02
                     PA[6:7]  | TIM3_CH[1:2] | AF02  */

        /* TIM3 clock enable */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

        {
            TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
            TIM_TimeBaseStructure.TIM_Period = 60000;
            TIM_TimeBaseStructure.TIM_Prescaler = 100;
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
        }

        /* GPIOA&C clock enable */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
        
        {
            GPIO_InitTypeDef GPIO_InitStructure;
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            GPIO_Init(GPIOA, &GPIO_InitStructure);

            /* Connect TIM pins to AF2 */
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
        }

        {
            GPIO_InitTypeDef GPIO_InitStructure;
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            GPIO_Init(GPIOC, &GPIO_InitStructure);

            /* Connect TIM pins to AF2 */
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
        }

        /* Enable the TIM3 global Interrupt */
        {
            NVIC_InitTypeDef NVIC_InitStructure;
            NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
            // TIM1_CC_IRQn 高级定时器与通用定时器的区别 中断号上 misc.h + stm32f4xx.h
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
        }

        /* TIM3 configuration: Input Capture mode */
        {
            TIM_ICInitTypeDef TIM_ICInitStructure;
            // TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2 | TIM_Channel_3 | TIM_Channel_4;
                // 自己看实现，虽然通道值设计允许 | 但是函数实现不允许 函数实现是if-else-if
            TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
            TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI,
            TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
            TIM_ICInitStructure.TIM_ICFilter = 0x0;

            TIM_ICInitStructure.TIM_Channel = TIM_Channel_1,
                TIM_ICInit(TIM3, &TIM_ICInitStructure);
            TIM_ICInitStructure.TIM_Channel = TIM_Channel_2,
                TIM_ICInit(TIM3, &TIM_ICInitStructure);
            TIM_ICInitStructure.TIM_Channel = TIM_Channel_3,
                TIM_ICInit(TIM3, &TIM_ICInitStructure);
            TIM_ICInitStructure.TIM_Channel = TIM_Channel_4,
                TIM_ICInit(TIM3, &TIM_ICInitStructure);
        }
        /* TIM enable counter */
        TIM_Cmd(TIM3, ENABLE);

        /* Enable the CC Interrupt Request */
        TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

}

void TIM3_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
    {
        static uint16_t ReadNO = 1;
        static uint16_t ReadValue1 = 0;
        static uint16_t ReadValue2 = 0;
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
        if (ReadNO == 1)
        {
            /* Get the Input Capture value */
            ReadValue1 = TIM_GetCapture1(TIM3);
            ReadNO = 2;
            TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);
        }
        else /* ReadNO == 2 */
        {
            /* Get the Input Capture value */
            ReadValue2 = TIM_GetCapture1(TIM3);
            
            /* Capture computation */
            if (ReadValue2 >= ReadValue1)
            {
                u16Tim3IC1 = (ReadValue2 - ReadValue1); 
            }
            else if (ReadValue2 < ReadValue1)
            {
                u16Tim3IC1 = ((0xFFFF - ReadValue1) + ReadValue2); 
            }
            ReadNO = 1;
            TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
        }
    }
    if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET) /***/
    {
        static uint16_t ReadNO = 1;
        static uint16_t ReadValue1 = 0;
        static uint16_t ReadValue2 = 0;
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2); /***/
        if (ReadNO == 1)
        {
            /* Get the Input Capture value */
            ReadValue1 = TIM_GetCapture2(TIM3); /***/
            ReadNO = 2;
            TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling); /***/
        }
        else /* ReadNO == 2 */
        {
            /* Get the Input Capture value */
            ReadValue2 = TIM_GetCapture2(TIM3); /***/
            
            /* Capture computation */
            if (ReadValue2 >= ReadValue1)
            {
                u16Tim3IC2 = (ReadValue2 - ReadValue1); /***/
            }
            else if (ReadValue2 < ReadValue1)
            {
                u16Tim3IC2 = ((0xFFFF - ReadValue1) + ReadValue2); /***/
            }
            ReadNO = 1;
            TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
        }
    }
    if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET) /***/
    {
        static uint16_t ReadNO = 1;
        static uint16_t ReadValue1 = 0;
        static uint16_t ReadValue2 = 0;
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3); /***/
        if (ReadNO == 1)
        {
            /* Get the Input Capture value */
            ReadValue1 = TIM_GetCapture3(TIM3); /***/
            ReadNO = 2;
            TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling); /***/
        }
        else /* ReadNO == 2 */
        {
            /* Get the Input Capture value */
            ReadValue2 = TIM_GetCapture3(TIM3); /***/
            
            /* Capture computation */
            if (ReadValue2 >= ReadValue1)
            {
                u16Tim3IC3 = (ReadValue2 - ReadValue1); /***/
            }
            else if (ReadValue2 < ReadValue1)
            {
                u16Tim3IC3 = ((0xFFFF - ReadValue1) + ReadValue2); /***/
            }
            ReadNO = 1;
            TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);
        }
    }
    if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET) /***/
    {
        static uint16_t ReadNO = 1;
        static uint16_t ReadValue1 = 0;
        static uint16_t ReadValue2 = 0;
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4); /***/
        if (ReadNO == 1)
        {
            /* Get the Input Capture value */
            ReadValue1 = TIM_GetCapture4(TIM3); /***/
            ReadNO = 2;
            TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling); /***/
        }
        else /* ReadNO == 2 */
        {
            /* Get the Input Capture value */
            ReadValue2 = TIM_GetCapture4(TIM3); /***/
            
            /* Capture computation */
            if (ReadValue2 >= ReadValue1)
            {
                u16Tim3IC4 = (ReadValue2 - ReadValue1); /***/
            }
            else if (ReadValue2 < ReadValue1)
            {
                u16Tim3IC4 = ((0xFFFF - ReadValue1) + ReadValue2); /***/
            }
            ReadNO = 1;
            TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising);
        }
    }
}
