
#include "inv_mpu.h"
#include "Attitude.h"

/* extern variables ---------------------------------------------------- */
short gyro[3], accel[3];                //六轴原始数据
float yaw, pitch, roll;                 //姿态角

void Attitude(void)
{
    while (mpu_dmp_get_data(&pitch, &roll, &yaw));
    // 函数内部把六轴原始数据放在全局变量 gyro 以及 accel 中
}
