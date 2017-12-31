个人NECLEO-F411RE固件库

# ./firmware/xxx

## FW1 Nucleo64-User:
	- Nucleo64板上的唯二两个用户外设
	- 蓝色的用户按钮 B1 - PC13
	- 绿色的用户LED LD2 - PA5 - TIM2_CH1
```
	void GPIO_init_4_LD2 (void);
	#define set_LD2(s)

	void GPIO_init_4_LD2 (void);
	#define set_LD2(s)

	s 接收 01
```

## FW2 delay:
	- 把原本官方工程的systick软延时提取过来了
	- 依赖systick
	- 实现上是 1 systick/ms，所以 delay 的参数就是要延时的毫秒数。
	- 同时维护一个全局变量 msTimerCounter ，可用来做外部计时。
```	
extern __IO uint32_t msTimerCounter;
void delayinit (void);
void delay (uint32_t ticks);
```

## FW3 LD2-PWM:
	- usage: 
		- 预定义 pwm_arr 宏
		- GPIO_init_4_LD2_PWM();
		- pwm_LD2 = pwm_range(x);
	- 绿色的用户LED LD2 - PA5 - TIM2_CH1
	- init过程用了库函数，修改直接就宏定义寄存器。
```
#define pwm_arr 100
#define pwm_LD2 (TIM2->CCR1)
#define pwm_range(x)
void GPIO_init_4_LD2_PWM (void);
```

## FW4 BTN-EXTI:
	- 蓝色的用户按钮 B1 - PC13 - EXTI_Line13 
```
void EXTI15_10_IRQHandler (void)
GPIO_init_4_BTN_EXTI();
// EXTI_GenerateSWInterrupt(EXTI_Line13);
```

## FW5 uart-nucleo:
	- usage:
		uart_init(115200);
		printf();
	- void USART2_IRQHandler(void)
	- fputc();
	- 用了原子哥的代码，大概知道是个怎么个流程，没有深究。

## FW6 FW6-motor-PWM:
	- usage:
		- motor_pwm_init();
		- motor_pwm_NO = motor_pwm_max; /min;
		- motor_pwm_NO = motor_pwm_range(motor_pwm_NO+delta);
		- NO = 1, 2, 3, 4
	- base on FW3
	- 电机编号如下图
```
         HEAD
       (X axis)
  (cw)M4      M1(acw)
       \      /
        \    /
         \--/
  Y      |  |
         /--\
        /    \
       /      \
 (acw)M3      M2(cw)
```
	- 对应转接板接口
		MTR[1:4]
	- 对应GPIO口
		PA[8:11]
	- 对应定时器通道(AF01)
		TIM1_CH[1:4]

## FW7 FW7-PWM-RCV
	- usage:
		- #include "motor-PWM.h"
		- RCV_IC_init();
		- read u16Rcvr_chx
	- u16Rcvr_chx in [1003, 2003]
	- FS-i6 遥控器默认通道对应关系
```
           ______           ______  
          /      \         /      \ 
         /        \       /        \
        (    O     )     (    O     )
         \        /       \        /
          \______/         \______/

  up/down  油门(3)         Y逆/顺时针(2)
left/right Z轴逆/顺时针(4)  X顺/逆时针(1)

  up/down  油门(3)         俯仰(2)
left/right 偏航(4)         滚转(1)

  up/down  Throttle(3)     Pitch(2)
left/right Yaw(4)          Roll(1)
```
	- 通道 103.5Hz 占空比 10%~20%
		- 然而在遥控器设置【RX Setup > Servos Freq】里面是50Hz..
		- 如果用 USB + NUCLEOF411 5V 给接受机供电，可能会电压不够，导致遥控器报警，这时候可以调整电压报警的阈值。
	- 接收机通道1-6对应转接板6-1
	- 对应GPIO口
		RCVr[1:2] |  RCV[6:5]  |  PA[7:6]
		RCVr[3:6] |  RCV[4:1]  |  PC[9:6]
	- 对码与验证方法
		第3通道油门直接接电调，12V电池给电调供电->电调5V给接收机供电。
		根据电调说明书给电调模式控制。
	- 对应定时器通道

|   GPIO   |   PCB    |    F411RE    | AFxx |
|----------|----------|--------------|------|
| PC[6:9]  | RCV[1:4] | TIM3_CH[1:4] | AF02 |
| PA[6:7]  | RCV[5:6] | TIM3_CH[1:2] | AF02 |
| PA[8:11] | MTR[1:4] | TIM1_CH[1:4] | AF01 |

## FW8-gpio-iic-mpu6050-rawdata
	- usage:
```
#include "gpio_mpu6050.h"

MPU_Init();

acc_correct();

gyro_correct();

read ax_cc, ay_cc, az_cc, gx_cc, gy_cc, gz_cc

```
	- 利用位带实现51类似的GPIO控制功能 (gpio_sys.h)
	- GPIO模拟IIC (gpio_i2c.x)
| GPIO | DO-What |
|------|---------|
| PB4  | SDL     |
| PB5  | SDA     |
	- 利用 gpio_i2c.x 完成对 mpu6050 的读写操作，并封装初始化和6轴原始数据的读取 (gpio_mpu6050.x)
