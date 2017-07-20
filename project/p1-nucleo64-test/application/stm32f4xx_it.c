
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

///@brief  This function handles NMI exception.
void NMI_Handler(void)
{
}

///@brief  This function handles Hard Fault exception.
void HardFault_Handler(void);

///@brief  This function handles Memory Manage exception.
void MemManage_Handler(void);

///@brief  This function handles Bus Fault exception.
void BusFault_Handler(void);

///@brief  This function handles Usage Fault exception.
void UsageFault_Handler(void);

///@brief  This function handles SVCall exception.
void SVC_Handler(void)
{
}

/// @brief  This function handles Debug Monitor exception.
void DebugMon_Handler(void)
{
}


//@brief  This function handles PendSVC exception.
//void PendSV_Handler(void)
//{
//}
//< defined in ucosii\os_cpu_a.asm

/// @brief  This function handles SysTick Handler.
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}
//#include "ucos_ii.h"
//void SysTick_Handler(void)
//{
//	if (OSRunning == OS_TRUE) {
//		OSIntEnter();
//		OSTimeTick();        
//		OSIntExit();
//	}
//}
//< use as os tick, if would like to run os

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

///@brief  This function handles PPP interrupt request.
/*void PPP_IRQHandler(void)*/
