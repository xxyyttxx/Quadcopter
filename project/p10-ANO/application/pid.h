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
    float iLimit;   // 积分值限制
    float Deriv;    // 微分值
    float Output;   // 输出
}PID_Typedef;

/* Exported functions ------------------------------------------------------- */
void PID_init(void);
void PID_calculate(void);
#if 0 //供扩展串级PID使用
void CtrlAttiAng(void);
void CtrlAttiRate(void);
#endif

/* Exported variables --------------------------------------------------------*/
extern PID_Typedef pitch_angle_PID; // pitch角度PID
extern PID_Typedef roll_angle_PID;
extern PID_Typedef yaw_angle_PID;

#if 0 //供扩展串级PID使用
extern PID_Typedef pitch_rate_PID;  // pitch角速度PID
extern PID_Typedef roll_rate_PID;
extern PID_Typedef yaw_rate_PID;
#endif

#endif /* _PID_H_ */
