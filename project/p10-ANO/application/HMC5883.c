#include "gpio_hmc5883.h"
#include "gpio_mpu6050.h"
#include "gpio_i2c.h"
#include "delay.h"

#define  HMC5883_Addr   0x1E

static uint8_t HMC_Write_Byte(uint8_t reg, uint8_t data)
{
    IIC_Start();
    IIC_Send_Byte((HMC5883_Addr<<1)|0);
    if(IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Send_Byte(data);
    if(IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
    return 0;
}

uint8_t InitHMC5883(void)
{
    // 主控制器的I2C与MPU6050的AUXI2C直通。控制器可以直接访问GY-86上的HMC5883L。即MPU6050不是AUXI2C线的主机。
    if (!MPU_Write_Byte(0X37, 0x82)) return 0;
    delay(20);
    return HMC_Write_Byte(0x00, 0x78) && HMC_Write_Byte(0x01, 0xE0) && HMC_Write_Byte(0x02, 0x01);
                        // 0 11 110 00                 111 00000                        单一测量模式
                        // 75Hz + 8次均值滤波           量程 +-8.1Ga ~ -2048~+2047 
                                                        // 230 Counts/Ga
}

void updateHMC5883(short mag[3])
{
    uint8_t buf[6];

    IIC_Start();
    IIC_Send_Byte((HMC5883_Addr<<1)|1);
    IIC_Wait_Ack();
    buf[0]=IIC_Read_Byte(1);
    buf[1]=IIC_Read_Byte(1);
    buf[2]=IIC_Read_Byte(1);
    buf[3]=IIC_Read_Byte(1);
    buf[4]=IIC_Read_Byte(1);
    buf[5]=IIC_Read_Byte(0);
    IIC_Stop();

    mag[0] = (buf[0] << 8) | buf[1]; // Combine MSB and LSB of X Data output register
    mag[1] = (buf[4] << 8) | buf[5]; // Combine MSB and LSB of Y Data output register
    mag[2] = (buf[2] << 8) | buf[3]; // Combine MSB and LSB of Z Data output register

    HMC_Write_Byte(0x02, 0x01);
}
