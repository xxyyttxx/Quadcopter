#ifndef __GPIO_SYS_H
#define __GPIO_SYS_H

#include "stm32f4xx.h"
#include <stdint.h>

// modify from 正点原子@ALIENTEK www.openedv.com

// 利用位带实现51类似的GPIO控制功能
                                            /* 位带的特定区域(常量)，一个字节映射到8个4字节(<<5 & <<2)上去 */
#define BITBAND(addr, bitnum)  ((addr & 0xF0000000)+0x2000000+((addr & 0xFFFFF)<<5)+(bitnum<<2))
                /* addr 不安全 */
#define MEM_ADDR(addr)         (*(volatile uint32_t *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

                        /* 寄存器偏移量 */
                        /* 20 == offset(GPIO_TypeDef, ODR) */
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014

                        /* 16 == offset(GPIO_TypeDef, IDR) */
#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010

/* 保证 n<16 */
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)

#endif /* __GPIO_SYS_H */
