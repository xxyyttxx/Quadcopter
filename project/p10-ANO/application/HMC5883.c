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

static uint8_t HMC_Read_Byte(uint8_t reg)
{
    uint8_t res;
    IIC_Start();
    IIC_Send_Byte((HMC5883_Addr<<1)|0);
    IIC_Wait_Ack();
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((HMC5883_Addr<<1)|1);
    IIC_Wait_Ack();
    res = IIC_Read_Byte(0);
    IIC_Stop();
    return res;
}


uint8_t InitHMC5883(void)
{
    // 主控制器的I2C与MPU6050的AUXI2C直通。控制器可以直接访问GY-86上的HMC5883L。即MPU6050不是AUXI2C线的主机。
    if (!MPU_Write_Byte(0X37, 0x82)) return 0;
    delay(20);
    return HMC_Write_Byte(0x02, 0x00) && HMC_Write_Byte(0x01, 0xE0);
}

// 量程 -4096~4095 -> -8~8 gz     0.5gz->250!
void updateHMC5883(short mag[3])
{
    unsigned char BUF[8];

    HMC_Write_Byte(0x00, 0x14);
    HMC_Write_Byte(0x02, 0x00);
    delay(10);

    BUF[1] = HMC_Read_Byte(0x03); // OUT_X_L_A
    BUF[2] = HMC_Read_Byte(0x04); // OUT_X_H_A
    BUF[3] = HMC_Read_Byte(0x07); // OUT_Y_L_A
    BUF[4] = HMC_Read_Byte(0x08); // OUT_Y_H_A
    BUF[5] = HMC_Read_Byte(0x05); // OUT_Z_L_A
    BUF[6] = HMC_Read_Byte(0x06); // OUT_Y_H_A

    mag[0] = (BUF[1] << 8) | BUF[2]; // Combine MSB and LSB of X Data output register
    mag[1] = (BUF[3] << 8) | BUF[4]; // Combine MSB and LSB of Y Data output register
    mag[2] = (BUF[5] << 8) | BUF[6]; // Combine MSB and LSB of Z Data output register
}

void hmc_correct(short mag_mid[3]){
    const int num=1000;
    short magRange[6]={0}; // bug fixed

    for(int i=0; i<num; i++)
    {
        short mag[3];
        updateHMC5883(mag);
        if(magRange[0] > mag[0]) magRange[0] = mag[0];
        if(magRange[1] < mag[0]) magRange[1] = mag[0];
        if(magRange[2] > mag[1]) magRange[2] = mag[1];
        if(magRange[3] < mag[1]) magRange[3] = mag[1];
        if(magRange[4] > mag[2]) magRange[4] = mag[2];
        if(magRange[5] < mag[2]) magRange[5] = mag[2];
        delay(6);   //6s
    }

    mag_mid[0] = (magRange[0]+magRange[1])/2;
    mag_mid[1] = (magRange[2]+magRange[3])/2;
    mag_mid[2] = (magRange[4]+magRange[5])/2;
}
