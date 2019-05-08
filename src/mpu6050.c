//File name:	mpu6050.c
//Author:	Dong Daming

#include "mpu6050.h"

//******************************************
//Name:		mpu6050_init
//Parameter:	void	
//Return:	fd	int	mpu6050 handle
//Description:	mpu6050 initization
//******************************************
int mpu6050_init(void)
{
	int fd;
	
	fd=i2c_init(0x68);

	i2c_writereg(fd,ADDR_PWR_MGMT_1,0x00);
	i2c_writereg(fd,ADDR_SMPLRT_DIV,0x07);
	i2c_writereg(fd,ADDR_CONFIG,0x06);
	i2c_writereg(fd,ADDR_GYRO_CONFIG,0x00);
	i2c_writereg(fd,ADDR_ACCEL_CONFIG,0x00);

	return fd;
}

//******************************************
//Name:		mpu6050_get
//Parameter:	fd	int	i2c handle
//		addr	uint8_t	register address
//Return:	data	uint16_treceived data
//Description:	read mpu6050 register
//******************************************
int16_t mpu6050_get(int fd,uint8_t addr)
{
	uint8_t data1,data2;
	uint16_t data;

	data1=i2c_readreg(fd,addr);
	data2=i2c_readreg(fd,addr+1);

	data=data1;
	data=(data<<8)+data2;

	return data;
}

void mpu6050_rd(int fd,double data[3])
{
	double accx,accz;

	data[1]=mpu6050_get(fd,ADDR_GYRO_YOUT)*250.0/0x7fff*PI/180;
	accx=mpu6050_get(fd,ADDR_ACCEL_XOUT)*2.0/0x7fff;
	accz=mpu6050_get(fd,ADDR_ACCEL_ZOUT)*2.0/0x7fff;

	data[2]=accx;
	if(accx>1)	accx=1;
	if(accx<-1)	accx=-1;

	//Use accelerometer yz axis data to calculate angle in full range.
	if(accz>0)			
		data[0]=-asin(accx);
	else if(accx<0)
		data[0]=-PI+asin(accx);
	else
		data[0]=PI+asin(accx);

}
