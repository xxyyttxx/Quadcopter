/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USARTx_IRQHANDLER   USART2_IRQHandler
#define TXBUFFERSIZE     (countof(aTxBuffer) - 1)
#define RXBUFFERSIZE     0x20

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t aTxBuffer[] = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal\
 communication using Interrupt\n\r";
uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t ubNbrOfDataToTransfer = TXBUFFERSIZE;
uint8_t ubNbrOfDataToRead = RXBUFFERSIZE;
__IO uint8_t ubTxCounter = 0;
__IO uint16_t uhRxCounter = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/**
    * @brief  This function handles USARTx global interrupt request.
    * @param  None
    * @retval None
    */
void USARTx_IRQHANDLER(void) /// NVIC 中断版本的收发
{
    if(USART_GetITStatus(MY_COM1, USART_IT_RXNE) != RESET)
    {
        /* Read one byte from the receive data register */
        aRxBuffer[uhRxCounter++] = (USART_ReceiveData(MY_COM1) & 0x7F);

        if(uhRxCounter == ubNbrOfDataToRead)
        {
            /* Disable the MY_COM1 Receive interrupt */
            USART_ITConfig(MY_COM1, USART_IT_RXNE, DISABLE);
        }
    }

    if(USART_GetITStatus(MY_COM1, USART_IT_TXE) != RESET)
    {
        /* Write one byte to the transmit data register */
        USART_SendData(MY_COM1, aTxBuffer[ubTxCounter++]);

        if(ubTxCounter == ubNbrOfDataToTransfer)
        {
            /* Disable the MY_COM1 Transmit interrupt */
            USART_ITConfig(MY_COM1, USART_IT_TXE, DISABLE);
        }
    }
}
