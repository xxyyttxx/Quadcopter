#include "stm32f4xx.h"
#include "EVAL_define.h"


USART_TypeDef* COM_USART[COMn] = {MY_COM1}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {MY_COM1_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {MY_COM1_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {MY_COM1_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {MY_COM1_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {MY_COM1_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {MY_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {MY_COM1_RX_PIN};
 
const uint16_t COM_TX_PIN_SOURCE[COMn] = {MY_COM1_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {MY_COM1_RX_SOURCE};
 
const uint16_t COM_TX_AF[COMn] = {MY_COM1_TX_AF};
 
const uint16_t COM_RX_AF[COMn] = {MY_COM1_RX_AF};


/**
    * @brief  Configures COM port.
    * @param  COM: Specifies the COM port to be configured.
    *   This parameter can be one of following parameters:    
    *     @arg COM1
    *     @arg COM2  
    * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
    *   contains the configuration information for the specified USART peripheral.
    * @retval None
    */
void MY_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

    if (COM == COM1)
    {
        /* Enable UART clock */
        RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    }

    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(COM_USART[COM], USART_InitStruct);
        
    /* Enable USART */
    USART_Cmd(COM_USART[COM], ENABLE);
}
