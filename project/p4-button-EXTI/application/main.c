#include "main.h"
#include "stm32f4xx_conf.h"
#include "btn_exti.h"
#include "LD2pwm.h"

/* Private typedef -----------------------------------------------------------*/
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
	
  /* Initialize LEDs mounted on EVAL board */
	GPIO_init_4_LD2_PWM();

	pwm_LD2 = pwm_arr;
	
  /* Configure EXTI Line13 in interrupt mode */
  GPIO_init_4_BTN_EXTI();
	
  /* Generate software interrupt: simulate a falling edge applied on EXTI0 line */
  EXTI_GenerateSWInterrupt(EXTI_Line13);

  /* Infinite loop */
  for (;;);
}
