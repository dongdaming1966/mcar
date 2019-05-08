#ifndef IMU_H
#define IMU_H

#include	"common.h"
#include	"math.h"

#include	"adis16405.h"
#include	"mpu6050.h"

int imu_init(void);
void imu_rd(int fd,double data[3]);

#endif //IMU_H
