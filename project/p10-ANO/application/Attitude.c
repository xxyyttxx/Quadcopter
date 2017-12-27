
#include "inv_mpu.h"
#include "Attitude.h"

float yaw, pitch, roll;                 //姿态角

void Attitude(void)
{
    while (mpu_dmp_get_data(&pitch, &roll, &yaw));
}
