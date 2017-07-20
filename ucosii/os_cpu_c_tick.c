/**
  @file    main.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   define tick-isr, which call OSTimeTick().
*/

#include "ucos_ii.h"
void SysTick_Handler (void)
{
	if (OSRunning == OS_TRUE) {
		OSIntEnter();
		OSTimeTick();
		OSIntExit();
	}
}
