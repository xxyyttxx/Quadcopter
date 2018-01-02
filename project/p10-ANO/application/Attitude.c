
#include "inv_mpu.h"
#include "Attitude.h"

/* extern variables ---------------------------------------------------- */
#define cn 10
short gyro[3], accel[3];                //六轴原始数据
float yaw, pitch, roll;                 //姿态角

void Attitude(void)
{
    while (mpu_dmp_get_data(&pitch, &roll, &yaw));

    static int gx_c, gy_c, gz_c;
    static short gx_w[cn], gy_w[cn], gz_w[cn];
    static int wid;
    gx_c -= gx_w[wid];
    gy_c -= gy_w[wid];
    gz_c -= gz_w[wid];

    gx_w[wid] = gyro[0];
    gy_w[wid] = gyro[1];
    gz_w[wid] = gyro[2];

    gx_c += gx_w[wid];
    gy_c += gy_w[wid];
    gz_c += gz_w[wid];

    if (++wid == cn) wid = 0;
    gyro[0] = gx_c/cn;
    gyro[1] = gy_c/cn;
    gyro[2] = gz_c/cn;

    // 函数内部把六轴原始数据放在全局变量 gyro 以及 accel 中
}
