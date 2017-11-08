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
	- 因为 extern uwTimingDelay，所以也可用来做计时。
	- "core_cm4.h" __IO __I __O -> volatile -> 防止多任务使用的资源被优化
	- 不是指针的参数返回值就算了
```	
extern __IO uint32_t uwTimingDelay;
void delayinit (void);
void delay (uint32_t ticks);
```

## FW3 LD2-PWM:
	- usage: 
		- modify pwm_arr
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

