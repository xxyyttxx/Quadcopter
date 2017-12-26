#include <math.h>
#include "Attitude.h"

#define AtR 0.0174533f
#define RtA 57.2956f

float kp = 100.0f;               // 比例增益支配率收敛到加速度
float ki = 0.1f;                 // 陀螺仪积分增益支配率
float halfT = 0.01f;             // 采样周期一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;   //四元数
float exInt = 0, eyInt = 0, ezInt = 0;  // 比例缩小积分误差
float yaw, pitch, roll;                 //姿态角

void Attitude(float gx, float gy, float gz, float ax, float ay, float az)
{

    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    //三轴加速度单位化
    norm = sqrt(ax*ax + ay*ay + az*az);
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    //转化为方向余弦矩阵第三行三个元素
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    //向量叉积运算用来修正陀螺仪和加速度之间的误差
    ex = (ay*vz - az*vy);
    ey = (az*vx - ax*vz);
    ez = (ax*vy - ay*vx);

    //积分求误差
    exInt = exInt + ex*ki;
    eyInt = eyInt + ey*ki;
    ezInt = ezInt + ez*ki;

    //修正陀螺仪
    gx = gx + kp*ex + exInt;
    gy = gy + kp*ey + eyInt;
    gz = gz + kp*ez + ezInt; /// short?

    //一阶龙格库塔法更新四元数
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + ( q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + ( q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + ( q0*gz + q1*gy - q2*gx)*halfT;

    //规范四元数
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    //四元数转欧拉角
    roll  = asin(-2.f * q1 * q3 + 2.f * q0 * q2)* RtA;
    pitch = atan2(2.f *(q2*q3 + q0*q1),q0*q0-q1*q1-q2*q2+q3*q3)* RtA;
    yaw   = atan2(2.f *(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3)* RtA;
}
