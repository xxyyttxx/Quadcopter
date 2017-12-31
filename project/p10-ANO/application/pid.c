#include "delay.h"
#include "pid.h"
#include "inv_mpu.h"
#include "Attitude.h"
#include "PWM-RCV.h"
#include "motor-PWM.h"


#define max_angle_pr 20
#define max_angle_yaw 180
#define I_limit_init 20
#define DMP_GYRO_SCALE 16.4f    // 2000deg/s , 31276/2000=16.4f


PID_Typedef pitch_angle_PID;    // pitch角度PID
PID_Typedef roll_angle_PID;
PID_Typedef yaw_angle_PID;

PID_Typedef pitch_rate_PID;
PID_Typedef roll_rate_PID;
PID_Typedef yaw_rate_PID;

extern short gyro[3];
extern float yaw, pitch, roll; // 观测角度，解算出来的

/**
    * @brief  PID初始化
    * @param  None
    * @retval None
    */

void PID_init(void)
{
    pitch_angle_PID.P = 2;
    pitch_angle_PID.I = 0;
    pitch_angle_PID.D = 65.535f;
    pitch_angle_PID.iLimit = I_limit_init;

    roll_angle_PID.P = 2;
    roll_angle_PID.I = 0;
    roll_angle_PID.D = 65.535f;
    roll_angle_PID.iLimit = I_limit_init;

    yaw_angle_PID.P = 1;
    yaw_angle_PID.I = 0;
    yaw_angle_PID.D = 0;
    yaw_angle_PID.iLimit = I_limit_init;
    
    pitch_rate_PID.P = 1;
    pitch_rate_PID.I = 0;
    pitch_rate_PID.D = 0;
    pitch_rate_PID.iLimit = I_limit_init;

    roll_rate_PID.P = 0;
    roll_rate_PID.I = 0;
    roll_rate_PID.D = 0;
    roll_rate_PID.iLimit = I_limit_init;

    yaw_rate_PID.P = 0;
    yaw_rate_PID.I = 0;
    yaw_rate_PID.D = 0;
    yaw_rate_PID.iLimit = I_limit_init;
}

/**
    * @brief  位置式PID：输入期望值、观测值，得到输出值
    * @param  PID_Typedef * PID： PID结构体
              float target：      期望值
              float measure：     观测值
              uint32_t dertT：     时间间隔，用于计算积分
    * @retval None
    */
static void PID_postion_cal(PID_Typedef * PID, float measure, uint32_t dertT)
{
    // 计算误差 = 期望值 - 观测值
    PID->Error = PID->Desired - measure;
    // 计算积分
    PID->Integ += (double)PID->Error * dertT / 1000.0;
// #if 0 // 暂时不给积分限制
    if (PID->Integ > PID->iLimit) PID->Integ = PID->iLimit;
    if (PID->Integ < -PID->iLimit) PID->Integ = -PID->iLimit; // 直接改累积合适吗?
    // 是否需要分段
// #endif
    // 计算微分
    PID->Deriv = PID->Error - PID->PreError;
    // 计算输出
    PID->Output = PID->P * PID->Error + PID->I * PID->Integ + PID->D * PID->Deriv;
    // 保留上一次误差
    PID->PreError = PID->Error;
}

static float range_trans(uint16_t Rcvr_ch, uint16_t max_angle)
{
    // 此函数要求通道捕获值为1000～2000
    if (Rcvr_ch<=1000) Rcvr_ch = 1001;
    if (Rcvr_ch>=2000) Rcvr_ch = 1999;
    return (Rcvr_ch-1500)/500.f*max_angle;
}

static inline uint16_t range_pwm(float motor_pwm, uint16_t thr)
{
    thr += motor_pwm;
    if (thr<=1000) thr = 1001;
    if (thr>=2000) thr = 1999;
    return thr;
}

#if 0 //供单环PID使用
void PID_calculate(void)
{
    static uint32_t told = 0;
    
    // 从遥控接收机获取期望值
    
    roll_angle_PID.Desired  = range_trans(u16Rcvr_ch1, max_angle_pr);       // f(u16Rcvr_ch1) +
    pitch_angle_PID.Desired = range_trans(u16Rcvr_ch2, max_angle_pr);       // f(u16Rcvr_ch2) +
    yaw_angle_PID.Desired   = 0; // range_trans(3000-u16Rcvr_ch4, max_angle_yaw); // f(u16Rcvr_ch4) -
                                     // !!!! // 假通道，没有需求
    uint32_t tnow = msTimerCounter;
    PID_postion_cal(&roll_angle_PID,  roll,  tnow-told); //要求平稳飞行 实际角度很小，0代表没有积分
    PID_postion_cal(&pitch_angle_PID, pitch, tnow-told);
    PID_postion_cal(&yaw_angle_PID,   yaw,   tnow-told);
    told = tnow;

    // 获取输出值
    uint16_t Thr = u16Rcvr_ch3; /// 不用转换量程，因为通道捕获和电调输出都是1000～2000
    float Pitch  = pitch_angle_PID.Output;
    float Roll   = roll_angle_PID.Output;
    float Yaw    = yaw_angle_PID.Output;

    // 输出值融合到四个电机
    if (Thr > 1100) { // 是否锁定状态
        motor_pwm_1 = range_pwm( - Pitch - Roll - Yaw, Thr);
        motor_pwm_2 = range_pwm( + Pitch - Roll + Yaw, Thr);
        motor_pwm_3 = range_pwm( + Pitch + Roll - Yaw, Thr);
        motor_pwm_4 = range_pwm( - Pitch + Roll + Yaw, Thr);
    } else {
        motor_pwm_1 = motor_pwm_min;
        motor_pwm_2 = motor_pwm_min;
        motor_pwm_3 = motor_pwm_min;
        motor_pwm_4 = motor_pwm_min;
    }
}
#endif

// 外环角度PID
void CtrlAttiAng(void)
{
    static uint32_t told = 0;
    
    roll_angle_PID.Desired  = range_trans(u16Rcvr_ch1, max_angle_pr);       // f(u16Rcvr_ch1) +
    pitch_angle_PID.Desired = range_trans(u16Rcvr_ch2, max_angle_pr);       // f(u16Rcvr_ch2) +
    yaw_angle_PID.Desired   = 0;                                            // 假通道，没有需求
    
    uint32_t tnow = msTimerCounter;
    PID_postion_cal(&roll_angle_PID,  roll,  tnow-told);
    PID_postion_cal(&pitch_angle_PID, pitch, tnow-told);
    PID_postion_cal(&yaw_angle_PID,   yaw,   tnow-told);
    told = tnow;
}

// 内环角速度PID
void CtrlAttiRate(void)
{
    static uint32_t told = 0;
    
    roll_rate_PID.Desired  = roll_angle_PID.Output ;
    pitch_rate_PID.Desired = pitch_angle_PID.Output;
    yaw_rate_PID.Desired   = yaw_angle_PID.Output  ;
    
    uint32_t tnow = msTimerCounter;
    PID_postion_cal(&roll_rate_PID,  gyro[0], tnow-told); // DMP_GYRO_SCALE
    PID_postion_cal(&pitch_rate_PID, gyro[1], tnow-told);
    PID_postion_cal(&yaw_rate_PID,   gyro[2], tnow-told);
    told = tnow;
    
    // 获取输出值
    uint16_t Thr = u16Rcvr_ch3; /// 不用转换量程，因为通道捕获和电调输出都是1000～2000
    float Pitch  = pitch_rate_PID.Output;
    float Roll   = roll_rate_PID.Output;
    float Yaw    = yaw_rate_PID.Output;

    // 输出值融合到四个电机
    if (Thr > 1100) { // 是否锁定状态
        motor_pwm_1 = range_pwm( - Pitch - Roll - Yaw, Thr);
        motor_pwm_2 = range_pwm( + Pitch - Roll + Yaw, Thr);
        motor_pwm_3 = range_pwm( + Pitch + Roll - Yaw, Thr);
        motor_pwm_4 = range_pwm( - Pitch + Roll + Yaw, Thr);
    } else {
        motor_pwm_1 = motor_pwm_min;
        motor_pwm_2 = motor_pwm_min;
        motor_pwm_3 = motor_pwm_min;
        motor_pwm_4 = motor_pwm_min;
    }
}

