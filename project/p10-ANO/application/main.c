/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "EVAL_define.h"
#include "delay.h"
#include "data_transfer.h"
#include "inv_mpu.h"
#include "PWM-RCV.h"
#include "motor-PWM.h"
#include "Attitude.h"
#include "pid.h"
void my2_ANO_DT_Data_Receive_Anl(void);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ndebug
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int i;
/* Private function prototypes -----------------------------------------------*/
static void USART_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
    * @brief  Main program
    * @param  None
    * @retval None
    */
int main(void)
{
    delayinit();
#ifndef ndebug
    motor_pwm_init();
    motor_pwm_1 = motor_pwm_2 = motor_pwm_3 = motor_pwm_4 = motor_pwm_max;
    delay(2000);
    motor_pwm_4 = motor_pwm_min;
    delay(1100);
    motor_pwm_1 = motor_pwm_2 = motor_pwm_3 = motor_pwm_min;
    delay(5000);
    RCV_IC_init();
#else
    delay(4000);
    motor_pwm_init();
    motor_pwm_1 = motor_pwm_2 = motor_pwm_3 = motor_pwm_4 = motor_pwm_min;
    delay(4000);
    motor_pwm_init();
    motor_pwm_1 = motor_pwm_2 = motor_pwm_3 = motor_pwm_4 = motor_pwm_min;
    delay(4000);
    RCV_IC_init();
#endif
    while (mpu_dmp_init());

    /* USART configuration */
    USART_Config();

    /* Enable the MY_COM1 Receive interrupt: this interrupt is generated when the
     MY_COM1 receive data register is not empty */
    USART_ITConfig(MY_COM1, USART_IT_RXNE, ENABLE);
	PID_init();
	
    for (i=0;;i++) {
        Attitude();
        CtrlAttiAng();
        CtrlAttiRate();
        ANO_DT_Data_Exchange();
        my2_ANO_DT_Data_Receive_Anl();
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
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = MY_COM1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

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
