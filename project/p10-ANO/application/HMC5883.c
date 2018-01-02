#include "gpio_hmc5883.h"
#include "gpio_i2c.h"
#include "delay.h"
#include "math.h"
#include "Attitude.h"

#define  HMC5883_Addr   0x1E

unsigned char BUF[8];

short magRange0,magRange1,magRange2,magRange3,magRange4,magRange5;
short magX, magY, magZ;
float magX_off, magY_off, magZ_off;
float magX_cc, magY_cc, magZ_cc;

uint8_t HMC_Write_Byte(uint8_t reg, uint8_t data)
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

uint8_t HMC_Read_Byte(uint8_t reg)
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


void InitHMC5883()
{

     HMC_Write_Byte(0x02,0x00);
     HMC_Write_Byte(0x01,0xE0);
 }


void updateHMC5883()
{


    HMC_Write_Byte(0x00,0x14);
    HMC_Write_Byte(0x02,0x00);
 //   Delayms(10);

    BUF[1]=HMC_Read_Byte(0x03);//OUT_X_L_A
    BUF[2]=HMC_Read_Byte(0x04);//OUT_X_H_A
    BUF[3]=HMC_Read_Byte(0x07);//OUT_Y_L_A
    BUF[4]=HMC_Read_Byte(0x08);//OUT_Y_H_A
    BUF[5]=HMC_Read_Byte(0x05);//OUT_Z_L_A
    BUF[6]=HMC_Read_Byte(0x06);//OUT_Y_H_A

    magX=(short)((BUF[1] << 8)|BUF[2]); //Combine MSB and LSB of X Data output register
    magY=(short)((BUF[3] << 8)|BUF[4]); //Combine MSB and LSB of Y Data output register
    magZ=(short)((BUF[5] << 8)|BUF[6]); //Combine MSB and LSB of Z Data output register

    magX=(magX-magX_off);
    magY=(magY-magY_off); // -4096~4095   ->   -8~8
    magZ=(magZ-magZ_off);



}

void hmc_correct(){
    int i=0;
    int num=1000;

    for(i=0;i<num;i++)
    {
        updateHMC5883();
        hmc_get_off(magX,magY,magZ);
        delay(6);   //6s
    }
}

void hmc_get_off(float mx, float my, float mz){

    if(magRange0 > mx) magRange0 = mx; // x min
    if(magRange1 < mx) magRange1 = mx; // x max
    if(magRange2 > my) magRange2 = my;
    if(magRange3 < my) magRange3 = my;
    if(magRange4 > mz) magRange4 = mz; // z min
    if(magRange5 < mz) magRange5 = mz; // z max
    magX_off = (magRange0+magRange1)/2.0;
    magY_off = (magRange2+magRange3)/2.0;
    magZ_off = (magRange4+magRange5)/2.0;
}
