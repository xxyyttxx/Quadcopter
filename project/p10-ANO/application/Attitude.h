#ifndef _ATTITUDE_H
#define _ATTITUDE_H

/* extern variables ---------------------------------------------------- */
extern short gyro[3], accel[3];  // 六轴原始数据
extern float yaw, pitch, roll;   // 姿态角

void Attitude(void);

#endif /* _ATTITUDE_H */
