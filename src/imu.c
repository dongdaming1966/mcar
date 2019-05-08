//File name:	imu.c	
//Author:	Dong Daming
//Hardware:	ADIS 16405

#include "imu.h"

//******************************************
//Name:		imu_init
//Parameter:	void
//Return:	fd	int	spi handle
//Description:	IMU initization
//******************************************
int imu_init(void)
{
	int fd;
	
//	fd=adis16405_init();
	fd=mpu6050_init();

	return fd;
}
 
//******************************************
//Name:		imu_rd
//Parameter:	fd	int	spi handle
//		data[3]	double	use to return the data received.
//				data[0]---->the angle between IMU z axis
//					    and vertical axis
//				data[1]---->gyro data
//				data[2]---->IMU power supply measurement
//Return:	void	
//******************************************
void imu_rd(int fd,double data[3])
{
//	adis16405_rd(fd,data);
	mpu6050_rd(fd,data);
}
                    
