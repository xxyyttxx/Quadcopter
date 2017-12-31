#include "stm32f4xx_conf.h"
#include "usart.h"
#include "delay.h"

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
    delayinit();
    delay(2000);
    /* Add your application code here */
    uart_init(38400);

#if 0 // test printf's efficiency
    char s[1024];
    for (int i=0;i<1023;i++) s[i]='a'; s[1023]=0;
    for (;;) {
        printf_("%s \r\n msCNT = %d \r\n", s, msTimerCounter); // 1024*9/38400 = 0.24s  实测0.309s
    }
#elif 1 // test delay & msCNT
    for (;;) {
        printf_("msCNT = %d \r\n", msTimerCounter); // 实际测试输出9次要1.017s
        delay(100);
    }
#endif

    /* Infinite loop */
    //for (;;);
}
