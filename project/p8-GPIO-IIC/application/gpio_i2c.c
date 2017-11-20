#include "gpio_i2c.h"
#include "gpio_sys.h"

// GPIO方向设置
#define SDA_IN()   do {GPIOB->MODER&=~(3<<(5*2));GPIOB->MODER|=0<<5*2;} while(0) //PB5输入模式
#define SDA_OUT()  do {GPIOB->MODER&=~(3<<(5*2));GPIOB->MODER|=1<<5*2;} while(0) //PB5输出模式

#define IIC_SCL    PBout(4)
#define IIC_SDA    PBout(5)
#define READ_SDA   PBin(5)

static void delay_us(volatile u32 nus)
{
  for (; nus > 0; nus--);
}

/* GPIO B4,B5 init */
void IIC_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  IIC_SCL=1;
  IIC_SDA=1; // 空闲状态
}

void IIC_Start(void)
{
    SDA_OUT();
    IIC_SDA=1; // 不信任调用者? 还是别的比如协议的原因
    IIC_SCL=1;
    delay_us(4);

    IIC_SDA=0; // START: when CLK is high, DATA change form high to low
    delay_us(4);
    IIC_SCL=0; // 钳住I2C总线，准备发送或接收数据
    delay_us(4);
}

void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL=0;
    delay_us(4);

    IIC_SDA=0;
    delay_us(4);
    IIC_SCL=1;
    delay_us(4);
    IIC_SDA=1; // STOP: when CLK is high DATA change form low to high
    delay_us(4);
}

uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;

    SDA_IN();
    IIC_SDA=1;
    delay_us(4);

    IIC_SCL=1;
    delay_us(4);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL=0;
    delay_us(4);
    return 0;
}

void IIC_Ack(void)
{
    SDA_OUT();
    IIC_SCL=0;
    delay_us(2);

    IIC_SDA=0;
    delay_us(2);
    IIC_SCL=1;
    delay_us(2);
    IIC_SCL=0;
    delay_us(2);
}

void IIC_NAck(void)
{
    IIC_SCL=0;
    SDA_OUT();
    delay_us(2);

    IIC_SDA=1;
    delay_us(2);
    IIC_SCL=1;
    delay_us(2);
    IIC_SCL=0;
    delay_us(2);
}

void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL=0; // 拉低时钟开始数据传输
    delay_us(2);
    for(t=0;t<8;t++)
    {
        IIC_SDA=txd>>7;
        txd<<=1;
        delay_us(2);
        IIC_SCL=1;
        delay_us(2);
        IIC_SCL=0;
        delay_us(2);
    }
}

uint8_t IIC_Read_Byte(uint8_t ack)
{
    unsigned char i,receive=0;
    SDA_IN();IIC_SCL=0; /// 0?
    for(i=0;i<8;i++ )
    {
        delay_us(2);
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;
        delay_us(2);
        IIC_SCL=0;
        delay_us(2);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}
