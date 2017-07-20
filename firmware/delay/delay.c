/**
  @file    delay.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   used systick to delay ms.
*/

#include "delay.h"

/// @brief  Inserts a delay time.
/// @param  nTime: specifies the delay time length, in milliseconds.
void Delay (__IO uint32_t ticks)
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
}

/// @brief  This function handles SysTick Handler.
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}


void delayinit (void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
}

