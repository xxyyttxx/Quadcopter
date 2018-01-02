#ifndef __GPIO_MPU6050_H
#define __GPIO_MPU6050_H
#include <stdint.h> // u8 define in stm32f4xx.h for legacy

uint8_t MPU_Init(void);

uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data);  // IIC写一个字节
uint8_t MPU_Read_Byte(uint8_t reg);                 // IIC读一个字节
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf); // IIC连续写
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);  // IIC连续读

#endif /* __GPIO_MPU6050_H */
