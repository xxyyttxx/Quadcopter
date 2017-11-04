这个目录下是 `ST 标准外设驱动库 ` `STM32F4 DSP and standard peripherals library`，这个库是芯片厂商 ST 提供的针对 STM32F4 系列的片上外设的驱动（当然也包含了 [CMSIS-CORE](https://cmsis.arm.com)、M4 架构接口）。本库的下载和配置见仓库的 README。协议见仓库的 LICENSE。

这个库其实已经很老了，早就可以被 HAL/LL 库替代了。

现在用的库的版本是 V1.8.0，也是现在 (171104) 的最新版本，好像很久没有更新过了。
其实 V1.4.0 也能用的，因为基本没有什么更新是和 F407 的外设相关的。
