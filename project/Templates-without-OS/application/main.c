/**
  @file    main.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20170720
  @brief   define main entry.
*/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  */
int main(void)
{
  for (;;);
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  for (;;);
}
#endif

//#include "ucos_ii.h"
//void SysTick_Handler(void)
//{
//  if (OSRunning == OS_TRUE) {
//    OSIntEnter();
//    OSTimeTick();        
//    OSIntExit();
//  }
//}
//< use as os tick, if would like to run os
