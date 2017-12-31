/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
 * 作者   ：匿名科创
 * 文件名  ：data_transfer.c
 * 描述    ：数据传输
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
**********************************************************************************/

/// modify by tjua in 171210

#include "data_transfer.h"
#include "gpio_mpu6050.h"
#include "PWM-RCV.h"
#include "motor-PWM.h"
#include "Attitude.h"
#include "pid.h"
#include <stdio.h>

#ifndef ndebug
static int ax_cc, ay_cc, az_cc;
static int gx_cc, gy_cc, gz_cc; // 硬解，没有读取六轴原始数据
#endif

void my1_ANO_DT_Data_Receive_Anl(u8 *RxBuffer, uint32_t length);

/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
// 小端 要求?
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

dt_flag_t f;            //需要发送数据的标志
u8 data_to_send[50];    //发送数据缓存 //// bug 没有上锁。。。

/////////////////////////////////////////////////////////////////////////////////////
//Data_Exchange函数处理各种数据发送请求，比如想实现每5ms发送一次传感器数据至上位机，即在此函数内实现
//此函数应由用户每1ms调用一次
void ANO_DT_Data_Exchange(void)
{
    static u8 cnt = 0;
    const u8 senser_cnt    = 8;
    const u8 status_cnt    = 16;
    const u8 rcdata_cnt    = 32;
    const u8 motopwm_cnt   = 32;
    const u8 power_cnt     = 64;
    const u8 F1_cnt        = 8;
    const u8 F2_cnt        = 8;

    if((cnt % senser_cnt) == (senser_cnt-1))
        f.send_senser = 1;

    if((cnt % status_cnt) == (status_cnt-1))
        f.send_status = 1;

    if((cnt % rcdata_cnt) == (rcdata_cnt-1))
        f.send_rcdata = 1;

    if((cnt % motopwm_cnt) == (motopwm_cnt-1))
        f.send_motopwm = 1;

    if((cnt % power_cnt) == (power_cnt-1))
        f.send_power = 1;

    if((cnt % F1_cnt) == (F1_cnt-1))
        f.send_F1 = 1;

    if((cnt % F2_cnt) == (F2_cnt-1))
        f.send_F2 = 1;

    cnt++;
/////////////////////////////////////////////////////////////////////////////////////
    if(f.send_version)
    {
        f.send_version = 0;
        ANO_DT_Send_Version(4,300,100,400,0);
    }
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_status)
    {
        f.send_status = 0;
        // void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);
        // ANO_DT_Send_Status(Roll,Pitch,Yaw,baroAlt,0,fly_ready);
        // 姿态结算数据
        ANO_DT_Send_Status(roll,pitch,yaw,121,122,1); // armed 是否上锁 0加锁/1解锁
    }
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_senser)
    {
        f.send_senser = 0;
        // void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar);
        // ANO_DT_Send_Senser(mpu6050.Acc.x,mpu6050.Acc.y,mpu6050.Acc.z,
        //                                      mpu6050.Gyro.x,mpu6050.Gyro.y,mpu6050.Gyro.z,
        //                                      ak8975.Mag_Adc.x,ak8975.Mag_Adc.y,ak8975.Mag_Adc.z,0);
        ANO_DT_Send_Senser(ax_cc, ay_cc, az_cc, gx_cc, gy_cc, gz_cc, 124, 125, 126, 0); // bar 压根没用上不知道有什么用
    }
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_rcdata)
    {
        f.send_rcdata = 0;
        // void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6);
        // ANO_DT_Send_RCData(Rc_Pwm_In[0],Rc_Pwm_In[1],Rc_Pwm_In[2],Rc_Pwm_In[3],Rc_Pwm_In[4],Rc_Pwm_In[5],Rc_Pwm_In[6],Rc_Pwm_In[7],0,0);
        ANO_DT_Send_RCData(u16Rcvr_ch3, u16Rcvr_ch4, u16Rcvr_ch1, u16Rcvr_ch2, motor_pwm_1, motor_pwm_2, motor_pwm_3, motor_pwm_4, 9, 10);
#ifdef debug_
        void Usart2_Send(uint8_t *data_to_send, uint32_t length);
        Usart2_Send("\r\nhere?,why no?\r\n", sizeof "\r\nhere?,why no?\r\n");
#endif
    }
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_motopwm)
    {
        f.send_motopwm = 0;
        // void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8);
        ANO_DT_Send_MotoPWM(motor_pwm_1, motor_pwm_2, motor_pwm_3, motor_pwm_4, 5,6,7,8);
    }
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_power)
    {
        f.send_power = 0;
        // void ANO_DT_Send_Power(u16 votage, u16 current);
        ANO_DT_Send_Power(12,0);
    }
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_pid1)
    {
        f.send_pid1 = 0;
        /*
        ANO_DT_Send_PID(1,ctrl_1.PID[PIDROLL].kp,ctrl_1.PID[PIDROLL].ki,ctrl_1.PID[PIDROLL].kd,
                          ctrl_1.PID[PIDPITCH].kp,ctrl_1.PID[PIDPITCH].ki,ctrl_1.PID[PIDPITCH].kd,
                          ctrl_1.PID[PIDYAW].kp,ctrl_1.PID[PIDYAW].ki,ctrl_1.PID[PIDYAW].kd);
        */
        // void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);
        ANO_DT_Send_PID(1, roll_angle_PID.P, roll_angle_PID.I, roll_angle_PID.D,
                           pitch_angle_PID.P, pitch_angle_PID.I, pitch_angle_PID.D,
                           yaw_angle_PID.P, yaw_angle_PID.I, yaw_angle_PID.D
                           );
    }
/////////////////////////////////////////////////////////////////////////////////////
    else if (f.send_F1) {
        f.send_F1 = 0;
        // void ANO_DT_Send_F1(float rcver_rol, float rcver_pit, float rcver_yaw,
        //             float error_rol, float error_pit, float error_yaw,
        //             float react_rol, float react_pit, float react_yaw);

        ANO_DT_Send_F1(roll_angle_PID.Desired, pitch_angle_PID.Desired, yaw_angle_PID.Desired,
                       roll_angle_PID.Error,   pitch_angle_PID.Error,   yaw_angle_PID.Error,
                       roll_angle_PID.Output,  pitch_angle_PID.Output,  yaw_angle_PID.Output
                       );
    }
/////////////////////////////////////////////////////////////////////////////////////
    else if (f.send_F2) {
        f.send_F2 = 0;
        // void ANO_DT_Send_F2(float Integ_Output_rol, float Integ_Output_pit, float Integ_Output_yaw);
        ANO_DT_Send_F2(roll_angle_PID.I * roll_angle_PID.Integ,
                       pitch_angle_PID.I * pitch_angle_PID.Integ,
                       yaw_angle_PID.I * yaw_angle_PID.Integ);
    }
/////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////
//Send_Data函数是协议中所有发送数据功能使用到的发送函数
//移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
void ANO_DT_Send_Data(u8 *dataToSend , u8 length)
{
    void Usart2_Send(uint8_t *, uint32_t);
    Usart2_Send(dataToSend, length);
}

static void ANO_DT_Send_Check(u8 head, u8 check_sum)
{
    static u8 data_to_send[10]; // 不能和外部用同一个缓冲区

    data_to_send[0]=0xAA;
    data_to_send[1]=0xAA;
    data_to_send[2]=0xEF;
    data_to_send[3]=2;
    data_to_send[4]=head;
    data_to_send[5]=check_sum;


    u8 sum = 0;
    for(u8 i=0;i<6;i++)
        sum += data_to_send[i];
    data_to_send[6]=sum;

    ANO_DT_Send_Data(data_to_send, 7);
}

/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare函数是协议预解析，根据协议的格式，将收到的数据进行一次格式性解析，格式正确的话再进行数据解析
//移植时，此函数应由用户根据自身使用的通信方式自行调用，比如串口每收到一字节数据，则调用此函数一次
//此函数解析出符合格式的数据帧后，会自行调用数据解析函数
void ANO_DT_Data_Receive_Prepare(u8 data)
{
    static u8 RxBuffer[50];
    static u8 _data_len = 0,_data_cnt = 0;
    static u8 state = 0;

    if(state==0&&data==0xAA)
    {
        state=1;
        RxBuffer[0]=data;
    }
    else if(state==1&&data==0xAF)
    {
        state=2;
        RxBuffer[1]=data;
    }
    else if(state==2&&data<0XF1)
    {
        state=3;
        RxBuffer[2]=data;
    }
    else if(state==3&&data<50)
    {
        state = 4;
        RxBuffer[3]=data;
        _data_len = data;
        _data_cnt = 0;
    }
    else if(state==4&&_data_len>0)
    {
        _data_len--;
        RxBuffer[4+_data_cnt++]=data;
        if(_data_len==0)
            state = 5;
    }
    else if(state==5)
    {
        state = 0;
        RxBuffer[4+_data_cnt]=data;
        my1_ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
    }
    else
        state = 0;
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl函数是协议数据解析函数，函数参数是符合协议格式的一个数据帧，该函数会首先对协议数据进行校验
//校验通过后对数据进行解析，实现相应功能
//此函数可以不用用户自行调用，由函数Data_Receive_Prepare自动调用
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num)
{
    u8 sum = 0;
    for(u8 i=0;i<(num-1);i++)
        sum += *(data_buf+i);
    if(!(sum==*(data_buf+num-1)))                       return;     //判断sum
    if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))     return;     //判断帧头

    if(*(data_buf+2)==0X02)
    {
        if(*(data_buf+4)==0X01)
        {
            f.send_pid1 = 1;
            f.send_pid2 = 1;
            f.send_pid3 = 1;
            f.send_pid4 = 1;
            f.send_pid5 = 1;
            f.send_pid6 = 1;
        }
        if(*(data_buf+4)==0XA0)     //读取版本信息
        {
            f.send_version = 1;
        }
    }

    if(*(data_buf+2)==0X10)                             //PID1
    {
        ANO_DT_Send_Check(*(data_buf+2),sum);
        // ctrl_1.PID[PIDROLL].kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        // ctrl_1.PID[PIDROLL].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        // ctrl_1.PID[PIDROLL].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        // ctrl_1.PID[PIDPITCH].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        // ctrl_1.PID[PIDPITCH].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        // ctrl_1.PID[PIDPITCH].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        // ctrl_1.PID[PIDYAW].kp   = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        // ctrl_1.PID[PIDYAW].ki   = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        // ctrl_1.PID[PIDYAW].kd   = 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
                //Param_SavePID(); 
        roll_angle_PID.P  = 0.001f * ( (*(data_buf+4)<<8) |*(data_buf+5)  );
        roll_angle_PID.I  = 0.001f * ( (*(data_buf+6)<<8) |*(data_buf+7)  );
        roll_angle_PID.D  =  0.01f * ( (*(data_buf+8)<<8) |*(data_buf+9)  );
        pitch_angle_PID.P = 0.001f * ( (*(data_buf+10)<<8)|*(data_buf+11) );
        pitch_angle_PID.I = 0.001f * ( (*(data_buf+12)<<8)|*(data_buf+13) );
        pitch_angle_PID.D =  0.01f * ( (*(data_buf+14)<<8)|*(data_buf+15) );
        yaw_angle_PID.P   = 0.001f * ( (*(data_buf+16)<<8)|*(data_buf+17) );
        yaw_angle_PID.I   = 0.001f * ( (*(data_buf+18)<<8)|*(data_buf+19) );
        yaw_angle_PID.D   =  0.01f * ( (*(data_buf+20)<<8)|*(data_buf+21) );

#ifdef debug_
        void Usart2_Send(uint8_t *data_to_send, uint32_t length);
        Usart2_Send("\r\nhere2\r\n", sizeof "\r\nhere2\r\n");
        static char s[20]; // Usart_Send 是排队并延迟到中断发送，不阻塞。。
        Usart2_Send((uint8_t *)s, snprintf(s, 100, "\r\n%d\r\n", (*(data_buf+20)<<8)|*(data_buf+21)));
#endif
    }
    if(*(data_buf+2)==0X11)                             //PID2
    {
        ANO_DT_Send_Check(*(data_buf+2),sum);
    }
    if(*(data_buf+2)==0X12)                             //PID3
    {
        ANO_DT_Send_Check(*(data_buf+2),sum);
    }
    if(*(data_buf+2)==0X13)                             //PID4
    {
        ANO_DT_Send_Check(*(data_buf+2),sum);
    }
    if(*(data_buf+2)==0X14)                             //PID5
    {
        ANO_DT_Send_Check(*(data_buf+2),sum);
    }
    if(*(data_buf+2)==0X15)                             //PID6
    {
        ANO_DT_Send_Check(*(data_buf+2),sum);
    }
}

void ANO_DT_Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver)
{
    u8 _cnt=0;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x00;
    data_to_send[_cnt++]=0;

    data_to_send[_cnt++]=hardware_type;
    data_to_send[_cnt++]=BYTE1(hardware_ver);
    data_to_send[_cnt++]=BYTE0(hardware_ver);
    data_to_send[_cnt++]=BYTE1(software_ver);
    data_to_send[_cnt++]=BYTE0(software_ver);
    data_to_send[_cnt++]=BYTE1(protocol_ver);
    data_to_send[_cnt++]=BYTE0(protocol_ver);
    data_to_send[_cnt++]=BYTE1(bootloader_ver);
    data_to_send[_cnt++]=BYTE0(bootloader_ver);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
    u8 _cnt=0;
    vs16 _temp;
    vs32 _temp2 = alt;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x01;
    data_to_send[_cnt++]=0;

    _temp = (int)(angle_rol*100);
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = (int)(angle_pit*100);
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = (int)(angle_yaw*100);
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    data_to_send[_cnt++]=BYTE3(_temp2);
    data_to_send[_cnt++]=BYTE2(_temp2);
    data_to_send[_cnt++]=BYTE1(_temp2);
    data_to_send[_cnt++]=BYTE0(_temp2);

    data_to_send[_cnt++] = fly_model;

    data_to_send[_cnt++] = armed;

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar)
{
    u8 _cnt=0;
    vs16 _temp;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x02;
    data_to_send[_cnt++]=0;

    _temp = a_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = a_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = a_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    _temp = g_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = g_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = g_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    _temp = m_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = m_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = m_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++] = sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
    u8 _cnt=0;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x03;
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=BYTE1(thr);
    data_to_send[_cnt++]=BYTE0(thr);
    data_to_send[_cnt++]=BYTE1(yaw);
    data_to_send[_cnt++]=BYTE0(yaw);
    data_to_send[_cnt++]=BYTE1(rol);
    data_to_send[_cnt++]=BYTE0(rol);
    data_to_send[_cnt++]=BYTE1(pit);
    data_to_send[_cnt++]=BYTE0(pit);
    data_to_send[_cnt++]=BYTE1(aux1);
    data_to_send[_cnt++]=BYTE0(aux1);
    data_to_send[_cnt++]=BYTE1(aux2);
    data_to_send[_cnt++]=BYTE0(aux2);
    data_to_send[_cnt++]=BYTE1(aux3);
    data_to_send[_cnt++]=BYTE0(aux3);
    data_to_send[_cnt++]=BYTE1(aux4);
    data_to_send[_cnt++]=BYTE0(aux4);
    data_to_send[_cnt++]=BYTE1(aux5);
    data_to_send[_cnt++]=BYTE0(aux5);
    data_to_send[_cnt++]=BYTE1(aux6);
    data_to_send[_cnt++]=BYTE0(aux6);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_Power(u16 votage, u16 current)
{
    u8 _cnt=0;
    u16 temp;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x05;
    data_to_send[_cnt++]=0;

    temp = votage;
    data_to_send[_cnt++]=BYTE1(temp);
    data_to_send[_cnt++]=BYTE0(temp);
    temp = current;
    data_to_send[_cnt++]=BYTE1(temp);
    data_to_send[_cnt++]=BYTE0(temp);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
    u8 _cnt=0;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x06;
    data_to_send[_cnt++]=0;

    data_to_send[_cnt++]=BYTE1(m_1);
    data_to_send[_cnt++]=BYTE0(m_1);
    data_to_send[_cnt++]=BYTE1(m_2);
    data_to_send[_cnt++]=BYTE0(m_2);
    data_to_send[_cnt++]=BYTE1(m_3);
    data_to_send[_cnt++]=BYTE0(m_3);
    data_to_send[_cnt++]=BYTE1(m_4);
    data_to_send[_cnt++]=BYTE0(m_4);
    data_to_send[_cnt++]=BYTE1(m_5);
    data_to_send[_cnt++]=BYTE0(m_5);
    data_to_send[_cnt++]=BYTE1(m_6);
    data_to_send[_cnt++]=BYTE0(m_6);
    data_to_send[_cnt++]=BYTE1(m_7);
    data_to_send[_cnt++]=BYTE0(m_7);
    data_to_send[_cnt++]=BYTE1(m_8);
    data_to_send[_cnt++]=BYTE0(m_8);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
    u8 _cnt=0;
    vu16 _temp;

    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x10+group-1;
    data_to_send[_cnt++]=0;


    _temp = p1_p * 1000;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = p1_i  * 1000;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = p1_d  * 100;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = p2_p  * 1000;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = p2_i  * 1000;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = p2_d * 100;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = p3_p  * 1000;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = p3_i  * 1000;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = p3_d * 100;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}

/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

void ANO_DT_Send_F1(float rcver_rol, float rcver_pit, float rcver_yaw,
                    float error_rol, float error_pit, float error_yaw,
                    float react_rol, float react_pit, float react_yaw)
{
    static u8 data_to_send[200];
    u8 _cnt=0;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xF1;
    data_to_send[_cnt++]=0;

    data_to_send[_cnt++]=BYTE3(rcver_rol);
    data_to_send[_cnt++]=BYTE2(rcver_rol);
    data_to_send[_cnt++]=BYTE1(rcver_rol);
    data_to_send[_cnt++]=BYTE0(rcver_rol);
    data_to_send[_cnt++]=BYTE3(rcver_pit);
    data_to_send[_cnt++]=BYTE2(rcver_pit);
    data_to_send[_cnt++]=BYTE1(rcver_pit);
    data_to_send[_cnt++]=BYTE0(rcver_pit);
    data_to_send[_cnt++]=BYTE3(rcver_yaw);
    data_to_send[_cnt++]=BYTE2(rcver_yaw);
    data_to_send[_cnt++]=BYTE1(rcver_yaw);
    data_to_send[_cnt++]=BYTE0(rcver_yaw);

    data_to_send[_cnt++]=BYTE3(error_rol);
    data_to_send[_cnt++]=BYTE2(error_rol);
    data_to_send[_cnt++]=BYTE1(error_rol);
    data_to_send[_cnt++]=BYTE0(error_rol);
    data_to_send[_cnt++]=BYTE3(error_pit);
    data_to_send[_cnt++]=BYTE2(error_pit);
    data_to_send[_cnt++]=BYTE1(error_pit);
    data_to_send[_cnt++]=BYTE0(error_pit);
    data_to_send[_cnt++]=BYTE3(error_yaw);
    data_to_send[_cnt++]=BYTE2(error_yaw);
    data_to_send[_cnt++]=BYTE1(error_yaw);
    data_to_send[_cnt++]=BYTE0(error_yaw);

    data_to_send[_cnt++]=BYTE3(react_rol);
    data_to_send[_cnt++]=BYTE2(react_rol);
    data_to_send[_cnt++]=BYTE1(react_rol);
    data_to_send[_cnt++]=BYTE0(react_rol);
    data_to_send[_cnt++]=BYTE3(react_pit);
    data_to_send[_cnt++]=BYTE2(react_pit);
    data_to_send[_cnt++]=BYTE1(react_pit);
    data_to_send[_cnt++]=BYTE0(react_pit);
    data_to_send[_cnt++]=BYTE3(react_yaw);
    data_to_send[_cnt++]=BYTE2(react_yaw);
    data_to_send[_cnt++]=BYTE1(react_yaw);
    data_to_send[_cnt++]=BYTE0(react_yaw);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}


void ANO_DT_Send_F2(float Integ_Output_rol, float Integ_Output_pit, float Integ_Output_yaw)
{
    static u8 data_to_send[200];
    u8 _cnt=0;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xF2;
    data_to_send[_cnt++]=0;

    data_to_send[_cnt++]=BYTE3(Integ_Output_rol);
    data_to_send[_cnt++]=BYTE2(Integ_Output_rol);
    data_to_send[_cnt++]=BYTE1(Integ_Output_rol);
    data_to_send[_cnt++]=BYTE0(Integ_Output_rol);
    data_to_send[_cnt++]=BYTE3(Integ_Output_pit);
    data_to_send[_cnt++]=BYTE2(Integ_Output_pit);
    data_to_send[_cnt++]=BYTE1(Integ_Output_pit);
    data_to_send[_cnt++]=BYTE0(Integ_Output_pit);
    data_to_send[_cnt++]=BYTE3(Integ_Output_yaw);
    data_to_send[_cnt++]=BYTE2(Integ_Output_yaw);
    data_to_send[_cnt++]=BYTE1(Integ_Output_yaw);
    data_to_send[_cnt++]=BYTE0(Integ_Output_yaw);

    data_to_send[3] = _cnt-4;

    u8 sum = 0;
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++]=sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
