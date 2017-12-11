/**
    @file    main.c
    @author  tjua @ ES-SS-UESTC © 2017
    @version V1.0
    @date    20171113
    @brief   define main entry.
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "gpio_mpu6050.h"


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
    uart_init(38400);
    MPU_Init();

    for (int c=0;;c++) {
        delay(1000);

        acc_correct();
        gyro_correct();

        printf("%d\r\n", c);
        printf("\tax_c = %d \r\n", ax_cc);
        printf("\tay_c = %d \r\n", ay_cc);
        printf("\taz_c = %d \r\n", az_cc);
        printf("\tgx_c = %d \r\n", gx_cc);
        printf("\tgy_c = %d \r\n", gy_cc);
        printf("\tgz_c = %d \r\n", gz_cc);
    }
}
