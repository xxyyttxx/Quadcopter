/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "EVAL_define.h"
#include "data_transfer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USARTx_IRQHANDLER   USART2_IRQHandler

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t *aTxBuffer;
static uint32_t ubNbrOfDataToTransfer;
static uint32_t ubTxCounter;
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
        ANO_DT_Data_Receive_Prepare(USART_ReceiveData(MY_COM1)); /// 之前这里有一个 & 0x7F：应该是用来处理 7O2 的，因为只有7个数据位，加上串口是LSB先发送，当时没有发现，理论上是要删掉的，当时由于只用
    }

    if(USART_GetITStatus(MY_COM1, USART_IT_TXE) != RESET)
    {
        /* Write one byte to the transmit data register */
        USART_SendData(MY_COM1, aTxBuffer[ubTxCounter]);

        if(++ubTxCounter == ubNbrOfDataToTransfer)
        {
            /* Disable the MY_COM1 Transmit interrupt */
            USART_ITConfig(MY_COM1, USART_IT_TXE, DISABLE);
        }
    }
}

void Usart2_Send(uint8_t *data_to_send, uint32_t length) {
    /* Wait until MY_COM1 send the TxBuffer */
    while (ubTxCounter < ubNbrOfDataToTransfer); // if os -> swi(控制资源) // 50*9/38400 = 0.0117s ...虽然是在自旋，但是可以被其他优先级更高的任务抢占
    /* The software must wait until TC=1. The TC flag remains cleared during all data
             transfers and it is set by hardware at the last frame’s end of transmission*/
    // while (USART_GetFlagStatus(MY_COM1, USART_FLAG_TC) == RESET);
    // USART_ClearFlag(MY_COM1, USART_FLAG_TC);

    aTxBuffer = data_to_send;
    ubNbrOfDataToTransfer = length;
    ubTxCounter = 0;
    /* Enable the MY_COM1 Transmit interrupt: this interrupt is generated when the
         MY_COM1 transmit data register is empty */
    USART_ITConfig(MY_COM1, USART_IT_TXE, ENABLE);
}

static u8 *RxBuf;
static uint32_t RxLen;
void my1_ANO_DT_Data_Receive_Anl(u8 *RxBuffer, uint32_t length) {
    if (!RxLen && !!length) {
        RxBuf = RxBuffer;
        RxLen = length;
        USART_ITConfig(MY_COM1, USART_IT_RXNE, DISABLE);
    } else ; // ignore this RxBuf
}
void my2_ANO_DT_Data_Receive_Anl() {
    if (RxLen) {
        ANO_DT_Data_Receive_Anl(RxBuf, RxLen);
        RxLen=0;
        USART_ITConfig(MY_COM1, USART_IT_RXNE, ENABLE);
    }
}
