#ifndef MPU6050_H
#define MPU6050_H

#include	"common.h"
#include	"config.h"
#include	"math.h"
#include       	"i2c.h" 

//use i2c_readreg
#define	ADDR_SMPLRT_DIV		0x19
#define	ADDR_CONFIG		0x1a
#define	ADDR_GYRO_CONFIG	0x1b
#define ADDR_ACCEL_CONFIG	0x1c
#define	ADDR_PWR_MGMT_1		0x6b

//use mpu6050_get
#define ADDR_ACCEL_XOUT		0x3b
#define ADDR_ACCEL_YOUT		0x3d
#define ADDR_ACCEL_ZOUT		0x3f
#define ADDR_GYRO_XOUT		0x43
#define ADDR_GYRO_YOUT		0x45
#define ADDR_GYRO_ZOUT		0x47

int mpu6050_init(void);
int16_t mpu6050_get(int fd,uint8_t addr);
void mpu6050_rd(int fd,double data[3]);
#endif //MPU6050_H

