author: [tjua](https://github.com/xxyyttxx), [daisenryaku](https://github.com/daisenryaku), [806216616](https://github.com/806216616) @ ES-SS-UESTC © 2017

## 项目介绍
一个基于stm32f411re以及ST标准库的四轴飞行器实现。

## 开发环境搭建

见[wiki介绍](https://github.com/UESTC404/Quadcopter/wiki/1.1-%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA)。

## 目录树结构与工程管理
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
	- 标准外设库配置：创建一个初始模版工程
		- 下载并解压 en.stm32f4_dsp_stdperiph_lib.zip
		- 确认版本号为 V1.8.0，否则可能需要改动脚步的部分内容
		- make create-stdp-proj
		- 一个工程的雏形就建立好了
		- 还需要针对 F411 修改部分代码
			1. 在 stdperiph/CMSIS/Device/stm32f4xx.h 增加 F411 宏定义 (而不使用编译选项)
			2. 修改用户代码 project/Templates/application/*

## PS: 工程结构对 github 不友好
- 其实这种文件结构对 github 很不友好，因为明明这个 project 依赖上层的 FW，FW 的修改却不能在本文件夹的 history 中展现。
- 这个问题的例子：P3 的 bug 是由 HW/BTN-EXTI 导致的，但是这个 bug 的修改在 github 的逻辑里和 P3 无关。这就很恼火了。
- github 工程文件夹有没有增加依赖关系的方法呀。难道这种一个仓库多个工程，还想复用用固件代码的人只有我一个异类？
- 依赖关系可以从头文件包含中得到；但是还是把 Keil 工程使用的 FW 清单写进 `project/readme.md` 比较合适。
