/**
  @file    main.c
  @author  tjua @ ES-SS-UESTC © 2017
  @version V1.0
  @date    20171113
  @brief   define main entry.
*/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "PWM-RCV.h"
#include "delay.h"
#include "usart.h"
#include "motor-PWM.h"


/* Private macro -------------------------------------------------------------*/
#define printf_(s, ...) printf("file:%s\tline:%d\t" s, __FILE__, __LINE__, __VA_ARGS__)

/* Private functions ---------------------------------------------------------*/

int main(void)
{
	
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files before to branch to application main.
       To reconfigure the default setting of SystemInit() function, 
       refer to system_stm32f4xx.c file */
	delayinit();
    motor_pwm_init();
    motor_pwm_1 = motor_pwm_2 = motor_pwm_3 = motor_pwm_4 = motor_pwm_max;
    delay(2000);
    motor_pwm_4 = motor_pwm_min;
    delay(1100);
    motor_pwm_1 = motor_pwm_2 = motor_pwm_3 = motor_pwm_min;
    // delay(5000);
  uart_init(38400);
	RCV_IC_init();

	for (int c=0;;c++) {
		delay(1000);
    printf("%d\r\n", c);
		printf("\tu16Rcvr_ch1 = %d \r\n", u16Rcvr_ch1);
    printf("\tu16Rcvr_ch2 = %d \r\n", u16Rcvr_ch2);
    printf("\tu16Rcvr_ch3 = %d \r\n", u16Rcvr_ch3);
    printf("\tu16Rcvr_ch4 = %d \r\n", u16Rcvr_ch4);
    printf("\tTIM3->CCR1 = %d \r\n", TIM3->CCR1);
    printf("\tTIM3->CCR2 = %d \r\n", TIM3->CCR2);
    printf("\tTIM3->CCR3 = %d \r\n", TIM3->CCR3);
    printf("\tTIM3->CCR4 = %d \r\n", TIM3->CCR4);

	}
}
