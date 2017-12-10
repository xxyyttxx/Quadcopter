/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EVAL_DEFINE_H
#define __EVAL_DEFINE_H

#include "stm32f4xx.h"

typedef enum 
{
    COM1 = 0
} COM_TypeDef;

#define COMn                             1

#define MY_COM1                        USART2
#define MY_COM1_CLK                    RCC_APB1Periph_USART2
#define MY_COM1_TX_PIN                 GPIO_Pin_2
#define MY_COM1_TX_GPIO_PORT           GPIOA
#define MY_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define MY_COM1_TX_SOURCE              GPIO_PinSource2
#define MY_COM1_TX_AF                  GPIO_AF_USART2
#define MY_COM1_RX_PIN                 GPIO_Pin_3
#define MY_COM1_RX_GPIO_PORT           GPIOA
#define MY_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define MY_COM1_RX_SOURCE              GPIO_PinSource3
#define MY_COM1_RX_AF                  GPIO_AF_USART2
#define MY_COM1_IRQn                   USART2_IRQn

void MY_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

#endif /* __EVAL_DEFINE_H */
