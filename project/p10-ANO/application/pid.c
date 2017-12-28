#include "pid.h"
#include "motor-PWM.h"

PID_Typedef pitch_angle_PID;	// pitch角度PID
PID_Typedef roll_angle_PID;
PID_Typedef yaw_angle_PID;

FLOAT_ANGLE EXP_ANGLE;         // 期望角度
extern float yaw, pitch, roll; // 观测角度，解算出来的
FLOAT_ANGLE DIF_ANGLE;         // 期望角度与测量角度的差

/**
    * @brief  PID初始化
    * @param  None
    * @retval None
    */

void PID_init(void) 
{
     pitch_angle_PID.P = 1;
     pitch_angle_PID.I = 0;
     pitch_angle_PID.D = 0;

     roll_angle_PID.P = 1;
     roll_angle_PID.I = 0;
     roll_angle_PID.D = 0;

     yaw_angle_PID.P = 1;
     yaw_angle_PID.I = 0;
     yaw_angle_PID.D = 0;
}

/**
    * @brief  位置式PID：输入期望值、观测值，得到输出值
    * @param  PID_Typedef * PID： PID结构体
    		  float target：      期望值
    		  float measure：     观测值
    		  int32_t dertT：     时间间隔，用于计算积分
    * @retval None
    */
void PID_postion_cal(PID_Typedef * PID, float target, float measure, int32_t dertT)
{
	// 计算误差 = 期望值 - 观测值
	PID->Error = target - measure;
	// 计算积分
	PID->Integ += (double)PID->Error * dertT / 1000000.0;
	// 计算微分
	PID->Deriv = PID->Error - PID->PreError;
	// 计算输出
	PID->Output = PID->P * PID->Error + PID->I * PID->Integ + PID->D * PID->Deriv;
	// 保留上一次误差
	PID->PreError = PID->Error;
}

void PID_calculate(void)
{  
	static float Thr=0, Roll=0, Pitch=0, Yaw=0;

	// 获取期望值
	EXP_ANGLE.Roll  = 0; // (float)u16Rcvr_ch1 ? 遥控接收机
	EXP_ANGLE.Pitch = 0;
	EXP_ANGLE.Yaw   = 0;

	// 获取误差值
	DIF_ANGLE.Roll  = EXP_ANGLE.Roll  - roll;
	DIF_ANGLE.Pitch = EXP_ANGLE.Pitch - pitch;
	DIF_ANGLE.Yaw   = EXP_ANGLE.Yaw   - pitch;

	PID_postion_cal(&roll_angle_PID,  EXP_ANGLE.Roll,  roll,  0); //没有积分
	PID_postion_cal(&pitch_angle_PID, EXP_ANGLE.Pitch, pitch, 0); 
	PID_postion_cal(&yaw_angle_PID,   EXP_ANGLE.Yaw,   yaw,   0);

	// 获取输出值
	Thr   = 1200; // 基础油门动力，(float)u16Rcvr_ch4 ?
	Pitch = pitch_angle_PID.Output;
	Roll  = roll_angle_PID.Output;
  Yaw   = yaw_angle_PID.Output;

  // 输出值融合到四个电机
  // motor_pwm_1 = (int16_t)(Thr - Pitch -Roll- Yaw ); 
  // motor_pwm_2 = 
  // motor_pwm_3 = 
  // motor_pwm_4 = 
}
