xmind + OG

# ./project/template-or-project-folder

## T1 Templates-from-stdp;
	- 来自 ST 外设库的移植过的可编译工程模版
	- 但是由于不是同一块芯片／外设，IO口不一样。所以就算是烧录进去了，也没有什么现象
	- 分析中断
	- SysTick end of count event each 10ms
		- RCC_ClocksTypeDef RCC_Clocks;
		- RCC_GetClocksFreq(&RCC_Clocks);
		- SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	- SysTick handle decrease counter for soft delay.
		- void SysTick_Handler(void) -> TimingDelay_Decrement();
	- 主函数应该是一个把时钟脉冲传给两个 LED。
	- 本应该的现象就是一个LED闪烁，另一个LED不知道怎么弄。

## P1 p1-nucleo64-test
	- 基于 T1，增加了很好玩的功能
	- 片上外设 GPIO + Nucleo 按钮和 LED
		- FW1/Nucleo64-User
		详情(接口与实现)见 FW/readme.md
	- 实现就是在软件延时delay死循环的时候增加按键检测，如果检测到按键按下并松开，那就把每次delay的延时double。
	- 现象就是你随着你按蓝色按钮，绿色LED会越闪越慢，从 0.1s->0.2s->0.4s->0.8s->1.6s->... (改变颜色的间隔) 摁了25次之后，后面会因为32位整形溢出出现不是倍增的结果；再继续会变成1/2PWM；
	- 电路上明明有C15这个消除抖动用的电容，但是实际在用的时候，还是有奇怪的现象（进Debug用watch看s，没有问题，应该是个人的错觉）
	- 下次注意：善用DEBUG，善用GIT(不应该在调通之后NEW COMMIT的)。

## T2 Templates-without-OS
	- 基于 T1
	- 不支持实时内核的新模版
	- 模仿标准外设库使用了keil管理源文件——禁用了UCOS2-GROUP
	- 这里的git就用的不错，copy new & modify commit
	- 大概有两三处代码结构上的改变吧

## P2 p2-led-pwm
	- 基于 T2
	- 片上外设 GPIO + LED + PWM
		- FW2/delay
		- FW3/LD2-PWM
	- 这个git虽然也是直接NEW，但是由于之前T2把重复的代码都删除后，new的都是新代码了。也就没有copy&diff的必要了。
	- 现象：线性LED亮度改变 (上升斜率:下降斜率 = 1:-2；寓意自己理解)
	- 希望改进：改成圆弧状的呼吸灯
	- 这里应该附图

## P3 p3-button-EXTI
	- 基于 T2 + 标准库示例
	- 增加用户按钮中断方法。
		- FW3/LD2-PWM
		- FW4/BTN-EXTI
	- git直接NEW，当年我怎么这么皮呀。
	- 带bug版本，bug在P4中修复了

## P4 p4-button-EXTI
	- 基于 P3
	- bugs fixed
	- 现象: 按住一次按钮或松开一次按钮，会产生一次中断使PWM占空比下降，表现为LED亮度下降。
	- 这里还有问题，就算有电容消除抖动，还是有一次按钮产生3~5~9次中断甚至更多。
	- 还是要加上延时消除抖动? 为什么这次debug就能看出一次按产生多次?

## P5 p5-uart-series-port
	- 基于 T2 + 标准库示例
		- FW5/uart-nucleo
	- 简单的测试串口
	- 115200 1 8 n
	- ls /dev/tty.*
	- minicom -h -s stlink
	- esc + Z

## P6 p6-PWM-motor
	- 基于 T2 & FW3
	- 电机编号
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
	- 对应定时器通道

## P7 p7-Capture-RCVR
	- FS-i6 遥控器通道对应关系
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
```
	- 接收机通道1-6对应转接板6-1
	- 对应GPIO口
		RCVr[1:2] |  RCV[6:5]  |  PA[7:6]
		RCVr[3:6] |  RCV[4:1]  |  PC[9:6]
	- 对码与验证方法
		第3通道油门直接接电调，12V电池给电调供电->电调5V给接收机供电。
		根据电调说明书给电调模式控制。
	- 对应定时器通道



ps. 新建工程的时候记得修改 `.uvprojx` 的名字
