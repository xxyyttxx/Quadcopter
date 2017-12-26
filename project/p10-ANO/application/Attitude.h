#ifndef _ATTITUDE_H
#define _ATTITUDE_H

#include "stm32f4xx.h"

extern float yaw, pitch, roll;   // 姿态角
void Attitude(short gx, short gy, short gz, short ax, short ay, short az);

#endif

