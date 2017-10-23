#!/bin/bash

# author: tjua @ ES-SS-UESTC © 2017

# 针对F411裁剪Lib&Templates

    ##########################################
    # ./stdperiph
    #  |....../CMSIS
    #  |      |....../Device
    #  |      |          <- STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/arm/startup_stm32f411xe.s
    #  |      |          <- STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    #  |      |          <- STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include/*
    #  |      |....../Include
    #  |                 <- STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Include/**
    #  |
    #  |....../Driver
    #         |          <- STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/STM32F4xx_StdPeriph_Driver/???
    #         |......./inc
    #         |......./src
    #
    ##########################################

    # Lib 代码

        # F411的启动代码
        mkdir -p stdperiph/CMSIS/Device; cp STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/arm/startup_stm32f411xe.s STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c stdperiph/CMSIS/Device

        # StdPeriph_Lib的主头文件（需要处理宏定义）
        mkdir -p stdperiph/CMSIS/Device; cp STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include/* stdperiph/CMSIS/Device

        # CM4的CMSIS-core
        mkdir -p stdperiph/CMSIS/Include; cp STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Include/core_cm4.h STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Include/core_cmFunc.h STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Include/core_cmInstr.h STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/CMSIS/Include/core_cmSimd.h stdperiph/CMSIS/Include

        # StdPeriph_Lib的片上外设分模块头文件；删除两个在F411宏定义下会出现问题的模块源文件
        mkdir -p stdperiph/Driver; cp -R STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Libraries/STM32F4xx_StdPeriph_Driver/??? stdperiph/Driver; rm stdperiph/Driver/src/stm32f4xx_fmc.c stdperiph/Driver/src/stm32f4xx_fsmc.c

    # Templates 用户代码
        mkdir -p project/Templates/application; cp STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Project/STM32F4xx_StdPeriph_Templates/main.c STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Project/STM32F4xx_StdPeriph_Templates/main.h STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_conf.h STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c STM32F4xx_DSP_StdPeriph_Lib_V1.8.0/Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.h project/Templates/application

# 针对F411修改部分代码

    # 修改用户代码
    touch project/Templates/application/*

    # 增加F411宏定义(不使用编译选项)
    touch stdperiph/CMSIS/Device/stm32f4xx.h
