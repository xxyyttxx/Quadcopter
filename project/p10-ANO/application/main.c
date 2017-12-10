/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_conf.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t ubNbrOfDataToTransfer;
extern uint8_t ubNbrOfDataToRead;
extern __IO uint8_t ubTxCounter;
extern __IO uint16_t uhRxCounter;

/* Private function prototypes -----------------------------------------------*/
static void NVIC_Config(void);
static void USART_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
    * @brief  Main program
    * @param  None
    * @retval None
    */
int main(void)
{

    /* NVIC configuration */
    NVIC_Config();

    /* USART configuration */
    USART_Config();

    /* Enable the MY_COM1 Transmit interrupt: this interrupt is generated when the
         MY_COM1 transmit data register is empty */
    USART_ITConfig(MY_COM1, USART_IT_TXE, ENABLE); /// 缓冲区+允许中断

    /* Wait until MY_COM1 send the TxBuffer */
    while(ubTxCounter < ubNbrOfDataToTransfer) /// 暴力循环等待（没有OS）
    {}

    /* The software must wait until TC=1. The TC flag remains cleared during all data
         transfers and it is set by hardware at the last frame’s end of transmission*/
    while (USART_GetFlagStatus(MY_COM1, USART_FLAG_TC) == RESET) /// 暴力循环等待2（没有OS）
    {}

    /* Enable the MY_COM1 Receive interrupt: this interrupt is generated when the
         MY_COM1 receive data register is not empty */
    USART_ITConfig(MY_COM1, USART_IT_RXNE, ENABLE); /// 缓冲区+允许中断

    /* Wait until MY_COM1 receive the RxBuffer */
    while(uhRxCounter < ubNbrOfDataToRead) /// 暴力循环等待（没有OS）
    {}

    while (1) /// 死循环
    {
    }
}

/**
    * @brief  Configures the USART Peripheral.
    * @param  None
    * @retval None
    */
static void USART_Config(void)
{
    USART_InitTypeDef USART_InitStructure;

/* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follows: 38400 8N1
                - BaudRate = 38400 baud
                - Word Length = 8 Bits
                - One Stop Bit
                - No parity
                - Hardware flow control disabled (RTS and CTS signals)
                - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    MY_COMInit(COM1, &USART_InitStructure);
}

/**
    * @brief  Configures the nested vectored interrupt controller.
    * @param  None
    * @retval None
    */
static void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = MY_COM1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
