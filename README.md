author: tjua @ ES-SS-UESTC © 2017

1. 开发环境
	- PD 虚拟机 + win7x64(update)
	- keil5.24a(不再支持 XP)：[Keil MDK-ARM](http://www.keil.com/mdk5)
	- STM32F4 芯片支持包：[Keil.STM32F4xx_DFP.2.11.0.pack](http://www.keil.com/dd2/pack/)

2. 目标硬件
	- NECLEO-F411RE：[介绍](http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f411re.html)
	- STM32F411RE：[介绍](http://www.st.com/content/st_com/en/products/microcontrollers/stm32-32-bit-arm-cortex-mcus/stm32-high-performance-mcus/stm32f4-series/stm32f411/stm32f411re.html)
	- 资料繁多、[重点](files)

3. 板载调试器驱动安装
	- [ST-link/v2](http://www.st.com/en/development-tools/debug-hardware-for-stm32-mcus.html)
	- [STSW-LINK008](http://www.st.com/content/st_com/en/products/development-tools/hardware-development-tools/development-tool-hardware-for-mcus/debug-hardware-for-mcus/debug-hardware-for-stm32-mcus/st-link-v2.html)
	- Windows 驱动：[en.stsw-link009.zip](http://www.st.com/content/st_com/en/products/embedded-software/development-tool-software/stsw-link008.html)

4. 标准外设库
	- [STSW-STM32065](http://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html)
	- [en.stm32f4_dsp_stdperiph_lib.zip](http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32065.html)
	- [License](http://www.st.com/software_license_agreement_liberty_v2)：自行查看源文件首部。

5. 实时系统
	- µc/os 官方移植的很不靠谱，最后用了航哥 (@daisenryaku) 那里拿的（感觉各种代码混合。很乱，但是能用）
	- [License](#)：自行查看源文件首部。

6. 工程约定：
	1. 不使用 Keil5 包管理
	2. 不使用 keil-target 管理 (不如文件管理方便)
	3. 使用标准外设库而非 [Cube 库](http://www.st.com/en/embedded-software/stm32cube-embedded-software.html)

7. 建立	 
	- 工程管理用了类似原子哥的形式，但改进了：多个工程 (project/application) 使用同一份驱动库代码 (ucosii/stdperiph/firmware)
	- 工程结构如下
		- stdperiph
			- CMSIS
			- Driver
		- ucosii
		- firmware
			- Nucleo64-LED
			- Nucleo64-Button
			- firmwareName3
			- ...
		- project
			- Templates
				- application
			- projectName2
				- application
			- ...
	- 标准外设库 -> 模版工程
		- 下载并解压 en.stm32f4_dsp_stdperiph_lib.zip
		- 确认版本号为 V1.8.0，否则可能需要改动脚步的部分内容
		- make create-stdp-proj
		- 一个工程的雏形就建立好了
		- 还需要针对 F411 修改部分代码
			1. 在 stdperiph/CMSIS/Device/stm32f4xx.h 增加 F411 宏定义 (而不使用编译选项)
			2. 修改用户代码 project/Templates/application/*

8. 工程结构对 github 不友好
	- 其实这种文件结构对 github 很不友好，因为明明这个 project 依赖上层的 FW，FW 的修改却不能在本文件夹的 history 中展现。
	- 这个问题的例子：P3 的 bug 是由 HW/BTN-EXTI 导致的，但是这个 bug 的修改在 github 的逻辑里和 P3 无关。这就很恼火了。
	- github 工程文件夹有没有增加依赖关系的方法呀。难道这种一个仓库多个工程，还想复用用固件代码的人只有我一个异类？
	- 依赖关系可以从头文件包含中得到；但是还是把 Keil 工程使用的 FW 清单写进 `project/readme.md` 比较合适。
