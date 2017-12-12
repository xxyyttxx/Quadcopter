#ifndef _ATTITUDE_H
#define _ATTITUDE_H

#include <math.h>
#include "stm32f4xx.h"
#include "MPU6050.h"
#include "delay.h"

void Attitude(short gx, short gy, short gz, short ax, short ay, short az);

#endif

