#ifndef _ATTITUDE_H
#define _ATTITUDE_H

#include "stm32f4xx.h"

extern float kp, ki, halfT;
extern float yaw, pitch, roll;   // 姿态角
void Attitude(float gx, float gy, float gz, float ax, float ay, float az);

#endif

