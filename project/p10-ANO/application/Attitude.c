
#include "inv_mpu.h"
#include "gpio_hmc5883.h"
#include "Attitude.h"
#include "math.h"

// #define nihetuoqiu
/* extern variables ---------------------------------------------------- */
#define cn 10
#define deg2rad(d) ((float)(d)/180.f*3.1415926f)
#define rad2deg(r) ((float)(r)/3.1415926f*180.f)
short gyro[3], accel[3], mag[3];        //九轴原始数据
short mag_mid[3] = {80, -379, 55};      //陀螺仪水平旋转校准数据 (特定系统)
float yaw, pitch, roll;                 //姿态角

void Attitude(void)
{
    // 函数内部把六轴原始数据放在全局变量 gyro 以及 accel 中
    while (mpu_dmp_get_data(&pitch, &roll, &yaw));

    // 获取电子罗盘原始数据，并做校准
    updateHMC5883(mag);
#ifndef nihetuoqiu
    mag[0] -= mag_mid[0];
    mag[1] -= mag_mid[1];
    mag[2] -= mag_mid[2];

    mag[0] /= 479/5000.f;
    mag[1] /= 483/5000.f;
    mag[2] /= 412/5000.f;

    // 用磁力计数据计算YAW角
    float rad_roll = deg2rad(roll);
    float cos_roll = cosf(rad_roll);
    float sin_roll = sinf(rad_roll);
    float rad_pitch = deg2rad(pitch);
    float cos_pitch = cosf(rad_pitch);
    float sin_pitch = sinf(rad_pitch);
    float sin_absroll = roll > 0 ? sin_roll : -sin_roll;
    float sin_abspitch = pitch > 0 ? sin_pitch : -sin_pitch;
    yaw = - rad2deg(
                atan2f(mag[1]*cos_roll  - mag[2]*sin_roll,
                       mag[0]*cos_pitch + mag[1]*sin_absroll*sin_abspitch + mag[2]*cos_roll*sin_pitch
                )
            ); // -180˚~180˚
#endif /* nihetuoqiu */

#if 0
    // 对陀螺仪数据做滑动窗口滤波
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
#endif
}
