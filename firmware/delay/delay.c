/**
  @file    delay.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   used systick to delay ms.
*/

#include "delay.h"

static __IO uint32_t uwTimingDelay;
volatile uint32_t msTimerCounter;
/* 
__IO的说明
定义在 "core_cm4.h"
__IO __I __O -> volatile -> 防止多任务(OS/IRQ)使用的资源被优化
不是指针的参数以及返回值就不要用这个修饰了
*/

/// @brief  Inserts a delay time.
/// @param  nTime: specifies the delay time length, in milliseconds.
void delay (uint32_t ticks)
{ 
  uwTimingDelay = ticks;

  while(uwTimingDelay != 0);
}

/// @brief  Decrements the TimingDelay variable.
static void TimingDelay_Decrement (void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
  msTimerCounter++;
}

/// @brief  This function handles SysTick Handler.
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}


void delayinit (void)
{
  /* SysTick end of count event each 1ms */
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000); // 见19/20/21讲笔记
}

