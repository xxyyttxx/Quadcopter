#include "delay.h"
#include "pid.h"
#include "inv_mpu.h"
#include "Attitude.h"
#include "PWM-RCV.h"
#include "motor-PWM.h"
#include "math.h"


#define max_angle_pr 5
#define max_rate_pr  20
#define I_limit_init 20
#define DMP_GYRO_SCALE 16.4f    // 2000deg/s , 31276/2000=16.4f


PID_Typedef pitch_angle_PID;    // pitch角度PID
PID_Typedef roll_angle_PID;
PID_Typedef yaw_angle_PID;

PID_Typedef pitch_rate_PID;
PID_Typedef roll_rate_PID;
PID_Typedef yaw_rate_PID;

extern float yaw, pitch, roll; // 观测角度，解算出来的

/**
    * @brief  PID初始化
    * @param  None
    * @retval None
    */

void PID_init(void)
{
    pitch_angle_PID.P = 3.2;
    pitch_angle_PID.I = 0.5;
    pitch_angle_PID.D = 0;
    pitch_angle_PID.iLimit = I_limit_init;

    roll_angle_PID.P = 4;
    roll_angle_PID.I = 0.7;
    roll_angle_PID.D = 0;
    roll_angle_PID.iLimit = I_limit_init;

    yaw_angle_PID.P = 8;
    yaw_angle_PID.I = 4;
    yaw_angle_PID.D = 0;
    yaw_angle_PID.iLimit = I_limit_init;

    pitch_rate_PID.P = 1.2;
    pitch_rate_PID.I = 0.5;
    pitch_rate_PID.D = 0;
    pitch_rate_PID.iLimit = I_limit_init;

    roll_rate_PID.P = 1.2;
    roll_rate_PID.I = 0.5;
    roll_rate_PID.D = 0;
    roll_rate_PID.iLimit = I_limit_init;

    yaw_rate_PID.P = 2;
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
    // 积分限制
    if (PID->Integ > PID->iLimit) PID->Integ = PID->iLimit;
    if (PID->Integ < -PID->iLimit) PID->Integ = -PID->iLimit;
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
    if (Rcvr_ch<=5)    Rcvr_ch = 1500; // ==0 接收机上电但是没有遥控器信号
    if (Rcvr_ch<=1000) Rcvr_ch = 1001;
    if (Rcvr_ch>=2000) Rcvr_ch = 1999;
    if (Rcvr_ch<=1510 && Rcvr_ch>=1490) Rcvr_ch = 1500; // 死区
    return (Rcvr_ch-1500)/500.f*max_angle;
}

static inline uint16_t range_pwm(float motor_pwm, uint16_t thr)
{
    thr += motor_pwm;
    if (thr<=1000) thr = 1001;
    if (thr>=2000) thr = 1999;
    return thr;
}

static inline uint16_t range_thr(uint16_t thr)
{
    return 600 + 0.6 * thr;
}

// 外环角度PID
void CtrlAttiAng(void)
{
    static uint32_t told = 0;

    uint32_t tnow = msTimerCounter;
    roll_angle_PID.Desired  = range_trans(u16Rcvr_ch1, max_angle_pr);       // f(u16Rcvr_ch1) +
    pitch_angle_PID.Desired = range_trans(u16Rcvr_ch2, max_angle_pr);       // f(u16Rcvr_ch2) +
    yaw_angle_PID.Desired  += range_trans(3000-u16Rcvr_ch4, max_rate_pr)*(tnow-told)/1000.f;   /// Question

    if (yaw_angle_PID.Desired > +180.f) yaw_angle_PID.Desired -= 360.f;
    if (yaw_angle_PID.Desired < -180.f) yaw_angle_PID.Desired += 360.f; // keep Desired in -180~180
    if (yaw_angle_PID.Desired - yaw > +180.f) yaw_angle_PID.Desired -= 360.f;
    if (yaw_angle_PID.Desired - yaw < -180.f) yaw_angle_PID.Desired += 360.f; // keep error smallest
    // abs(error) > max_angle_pr ? Desired = yaw :  ;
    if (fabs(yaw_angle_PID.Desired - yaw) > max_angle_pr) yaw_angle_PID.Desired = yaw + copysignf(max_angle_pr, yaw_angle_PID.Desired - yaw);
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

    //roll_rate_PID.Desired  = range_trans(u16Rcvr_ch1, 100);
    //pitch_rate_PID.Desired = range_trans(u16Rcvr_ch2, 100);
    //yaw_rate_PID.Desired   = range_trans(3000-u16Rcvr_ch4, 60);

    uint32_t tnow = msTimerCounter;
    PID_postion_cal(&roll_rate_PID,  gyro[0] / DMP_GYRO_SCALE, tnow-told); // DMP_GYRO_SCALE
    PID_postion_cal(&pitch_rate_PID, gyro[1] / DMP_GYRO_SCALE, tnow-told);
    PID_postion_cal(&yaw_rate_PID,   gyro[2] / DMP_GYRO_SCALE, tnow-told);
    told = tnow;
}

void CtrlMotorSpeed(void)
{
    // 获取输出值
    uint16_t Thr = u16Rcvr_ch3; /// 不用转换量程，因为通道捕获和电调输出都是1000～2000
    float Pitch  = pitch_rate_PID.Output;
    float Roll   = roll_rate_PID.Output;
    float Yaw    = yaw_rate_PID.Output;

    // 输出值融合到四个电机
    if (Thr > 1100) { // 是否锁定状态
        motor_pwm_1 = range_pwm( - Pitch - Roll - Yaw, range_thr(Thr));
        motor_pwm_2 = range_pwm( + Pitch - Roll + Yaw, range_thr(Thr));
        motor_pwm_3 = range_pwm( + Pitch + Roll - Yaw, range_thr(Thr));
        motor_pwm_4 = range_pwm( - Pitch + Roll + Yaw, range_thr(Thr));
    } else {
        motor_pwm_1 = motor_pwm_min;
        motor_pwm_2 = motor_pwm_min;
        motor_pwm_3 = motor_pwm_min;
        motor_pwm_4 = motor_pwm_min;
    }
}
