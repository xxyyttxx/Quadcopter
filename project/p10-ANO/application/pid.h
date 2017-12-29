#ifndef _PID_H_
#define _PID_H_

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    float P, I, D;  // PID三个参数
    float Desired;  // 期望
    float Error;    // 误差
    float PreError; // 上一次误差
    float Integ;    // 积分值
    float Deriv;    // 微分值
    float Output;   // 输出
}PID_Typedef;

typedef struct float_angle
{
    float Roll;
    float Pitch;
    float Yaw;
}FLOAT_ANGLE;

/* Exported functions ------------------------------------------------------- */
void PID_init(void);
void PID_calculate(void);

/* Exported variables --------------------------------------------------------*/
extern PID_Typedef pitch_angle_PID; // pitch角度PID
extern PID_Typedef roll_angle_PID;
extern PID_Typedef yaw_angle_PID;

#if 0 // pitch角速度PID，供扩展串级PID使用
extern PID_Typedef pitch_rate_PID;
extern PID_Typedef roll_rate_PID;
extern PID_Typedef yaw_rate_PID;
#endif

extern FLOAT_ANGLE EXP_ANGLE; // 期望角度
extern FLOAT_ANGLE DIF_ANGLE; // 期望角度与测量角度的差

#endif /* _PID_H_ */
