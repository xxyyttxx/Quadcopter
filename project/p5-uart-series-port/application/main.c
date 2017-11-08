#include "stm32f4xx_conf.h"
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/
#define printf_(s, ...) printf("file:%s\tline:%d\t" s, __FILE__, __LINE__, __VA_ARGS__)
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @retval None
  */
int main(void)
{
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files before to branch to application main.
       To reconfigure the default setting of SystemInit() function, 
       refer to system_stm32f4xx.c file */

  /* Add your application code here */
	int c = 0;
	uart_init(115200);
	for (;;) {
		printf_("c = %d \r\n", c++);
	}

  /* Infinite loop */
  //for (;;);
}
