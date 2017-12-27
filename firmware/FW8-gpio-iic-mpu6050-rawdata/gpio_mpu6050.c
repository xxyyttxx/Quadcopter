#include "gpio_mpu6050.h"
#include "gpio_i2c.h"
#include "delay.h"

#define windows_fliter

////////////////////////////////////////////////////////////////////////////////


/* MPU6050 寄存器地址 */

#define MPU_SELF_TESTX_REG      0X0D    // 自检寄存器X
#define MPU_SELF_TESTY_REG      0X0E    // 自检寄存器Y
#define MPU_SELF_TESTZ_REG      0X0F    // 自检寄存器Z
#define MPU_SELF_TESTA_REG      0X10    // 自检寄存器A
#define MPU_SAMPLE_RATE_REG     0X19    // 采样频率分频器
#define MPU_CFG_REG             0X1A    // 配置寄存器
#define MPU_GYRO_CFG_REG        0X1B    // 陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG       0X1C    // 加速度计配置寄存器
#define MPU_MOTION_DET_REG      0X1F    // 运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG         0X23    // FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG     0X24    // IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG    0X25    // IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG         0X26    // IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG    0X27    // IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG    0X28    // IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG         0X29    // IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG    0X2A    // IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG    0X2B    // IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG         0X2C    // IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG    0X2D    // IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG    0X2E    // IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG         0X2F    // IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG    0X30    // IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG    0X31    // IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG         0X32    // IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG      0X33    // IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG    0X34    // IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG      0X35    // IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG      0X36    // IIC主机状态寄存器
#define MPU_INTBP_CFG_REG       0X37    // 中断/旁路设置寄存器
#define MPU_INT_EN_REG          0X38    // 中断使能寄存器
#define MPU_INT_STA_REG         0X3A    // 中断状态寄存器

#define MPU_ACCEL_XOUTH_REG     0X3B    // 加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG     0X3C    // 加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG     0X3D    // 加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG     0X3E    // 加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG     0X3F    // 加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG     0X40    // 加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG       0X41    // 温度值高八位寄存器
#define MPU_TEMP_OUTL_REG       0X42    // 温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG      0X43    // 陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG      0X44    // 陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG      0X45    // 陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG      0X46    // 陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG      0X47    // 陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG      0X48    // 陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG      0X63    // IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG      0X64    // IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG      0X65    // IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG      0X66    // IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG    0X67    // IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG     0X68    // 信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG    0X69    // 运动检测控制寄存器
#define MPU_USER_CTRL_REG       0X6A    // 用户控制寄存器
#define MPU_PWR_MGMT1_REG       0X6B    // 电源管理寄存器1
#define MPU_PWR_MGMT2_REG       0X6C    // 电源管理寄存器2
#define MPU_FIFO_CNTH_REG       0X72    // FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG       0X73    // FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG         0X74    // FIFO读写寄存器
#define MPU_DEVICE_ID_REG       0X75    // 器件ID寄存器

#define MPU_ADDR                0X68    // AD0接地,IIC地址为0X68(不包含最低位).


////////////////////////////////////////////////////////////////////////////////


/* MPU6050 控制接口，更底层是 IIC + GPIO */

static uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data);  // IIC写一个字节
static uint8_t MPU_Read_Byte(uint8_t reg);                 // IIC读一个字节

static uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr);
static uint8_t MPU_Set_Accel_Fsr(uint8_t fsr);

static uint8_t MPU_Set_LPF(uint16_t lpf);
static uint8_t MPU_Set_Rate(uint16_t rate);
static uint8_t MPU_Set_Fifo(uint8_t sens);

static short MPU_Get_Temperature(void);
static uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz);
static uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az);


////////////////////////////////////////////////////////////////////////////////


/* 量程相关函数 */

/** @brief 设置 MPU6050 陀螺仪传感器满量程范围.
    @param fsr: 
        0, ±250dps;
        1, ±500dps;
        2, ±1000dps;
        3, ±2000dps(°/sec)
    @retval int
            0,      设置成功
            others, 设置失败
*/

uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr<<3); // 设置陀螺仪满量程范围
}

/** @brief 设置 MPU6050 加速度传感器满量程范围.
    @param fsr:
        0, ±2g;
        1, ±4g;
        2, ±8g;
        3, ±16g(重力加速度)
    @retval int
            0,      设置成功
            others, 设置失败
    @notes 2^15 = 32768
*/
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr<<3); // 设置加速度传感器满量程范围
}


////////////////////////////////////////////////////////////////////////////////


/** @brief 设置MPU6050的数字低通滤波器.
    @param lpf: 数字低通滤波频率(Hz)
    @retval int
            0,      设置成功
            others, 设置失败
*/
uint8_t MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data=0;
    if      (lpf>=188) data=1;
    else if (lpf>=98)  data=2;
    else if (lpf>=42)  data=3;
    else if (lpf>=20)  data=4;
    else if (lpf>=10)  data=5;
    else data=6;
    return MPU_Write_Byte(MPU_CFG_REG, data);//设置数字低通滤波器
}


/** @brief 设置MPU6050的采样率(假定Fs=1KHz)
    @param rate: 4~1000(Hz)
    @retval int
            0,      设置成功
            others, 设置失败
*/
uint8_t MPU_Set_Rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data = 1000/rate-1;

    /* 设置采样率 以及 设置数字低通滤波器频率为采样率的一半 */
    return MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data) || MPU_Set_LPF(rate/2);
}


////////////////////////////////////////////////////////////////////////////////


/* 获取重要的数据寄存器的值 */

/** @brief 获取MPU6050温度.
    @retval 温度寄存器的值
*/
short MPU_Get_Temperature(void)
{
    uint8_t buf[2];
    MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG, 2, buf);
    short raw = ((uint16_t)buf[0]<<8)|buf[1];
    float temp = 36.53+((double)raw)/340;
    return temp*100;
}

/** @brief 得到陀螺仪值(原始值).
    @param gx, gy, gz: 陀螺仪三轴的原始读数(带符号)
    @retval int
            0,      读取成功
            others, 读取失败
*/
uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    uint8_t buf[6], res;
    if ((res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf)) == 0) {
        *gx=(((uint16_t)buf[0]<<8)|buf[1]);;
        *gy=(((uint16_t)buf[2]<<8)|buf[3]);;
        *gz=(((uint16_t)buf[4]<<8)|buf[5]);;
    }
    return res;
}

/** @brief 得到加速度值(原始值).
    @param gx, gy, gz: 加速度三轴的原始读数(带符号)
    @retval int
            0,      读取成功
            others, 读取失败
*/
uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
    uint8_t buf[6], res;
    if((res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf)) == 0) {
        *ax=((uint16_t)buf[0]<<8)|buf[1];
        *ay=((uint16_t)buf[2]<<8)|buf[3];
        *az=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;
}


////////////////////////////////////////////////////////////////////////////////


/* MPU6050的IIC读写时序 */


/** @brief IIC连续写.
    @param  addr: 器件地址
            reg:  寄存器地址
            len:  写入长度
            buf:  数据区
    @retval int
            1 成功
            0 失败
*/
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t i;
    IIC_Start();
    IIC_Send_Byte((addr<<1)|0);
    if(IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    for(i=0;i<len;i++)
    {
        IIC_Send_Byte(buf[i]);
        if(IIC_Wait_Ack())
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_Stop();
    return 0;
}


/** @brief IIC连续读.
    @param 
        addr: 器件地址
        reg: 要读取的寄存器地址
        len: 要读取的长度
        buf: 读取到的数据存储区
    @retval int
            1 成功
            0 失败
*/
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    IIC_Start();
    IIC_Send_Byte((addr<<1)|0);
    if(IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((addr<<1)|1);
    IIC_Wait_Ack();
    while(len)
    {
        if(len==1)*buf=IIC_Read_Byte(0);
        else *buf=IIC_Read_Byte(1);
        len--;
        buf++;
    }
    IIC_Stop();
    return 0;
}


/** @brief IIC写一个字节.
    @param 
        reg: 寄存器地址
        data: 数据
    @retval int
            1 成功
            0 失败
*/
uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data)
{
    IIC_Start();
    IIC_Send_Byte((MPU_ADDR<<1)|0);
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


/** @brief IIC读一个字节.
    @param 
        reg: 寄存器地址
    @retval uint8_t 读到的数据
*/
uint8_t MPU_Read_Byte(uint8_t reg)
{
    uint8_t res;
    IIC_Start();
    IIC_Send_Byte((MPU_ADDR<<1)|0);
    IIC_Wait_Ack();
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((MPU_ADDR<<1)|1);
    IIC_Wait_Ack();
    res = IIC_Read_Byte(0);
    IIC_Stop();
    return res;
}


////////////////////////////////////////////////////////////////////////////////


/* 函数接口 */


/** @brief 通过IIC初始化MPU6050.
    @retval uint8_t
            0 初始化正常
            1 初始化失败
    @notes  这里是通过什么检查的呢，感觉这里的错误反馈都是假的
            不正常运行也不知道为什么
*/
uint8_t MPU_Init(void)
{
    /* 初始化IIC总线 */
    IIC_Init();
    /* 复位MPU6050 */
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80);
    delay(100);

    /* 唤醒MPU6050 */
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00);

    /* 设置量程和采样率 */
    MPU_Set_Gyro_Fsr(3);                      /* 陀螺仪传感器, ±2000dps 16.4C */
    MPU_Set_Accel_Fsr(2);                     /* 加速度传感器, ±8g 4096 */
    MPU_Set_Rate(125);                        /* 设置采样率125Hz */

    /* 关闭各种无用模式功能，不用MPU */
    MPU_Write_Byte(MPU_INT_EN_REG, 0X00);     /* 关闭所有中断 */
    MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00);  /* I2C主模式关闭 */
    MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);    /* 关闭FIFO */
    MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80);  /* INT引脚低电平有效 */

    /* 检查器件ID */ // 为什么在这里
    if (MPU_Read_Byte(MPU_DEVICE_ID_REG) != MPU_ADDR)
        return 1; /* 器件ID不正确，直接返回 */

    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01); /* 设置CLKSEL, PLL X轴为参考 */
    MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); /* 加速度与陀螺仪都工作 */
    MPU_Set_Rate(250);                       /* 设置采样率为50Hz */
    return 0;
}
