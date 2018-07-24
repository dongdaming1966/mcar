//Projact Name:	mcar
//Author:	Dong Daming
//Hardware:	Controller:	raspberry pi3
//		IMU:		ADIS 16405
//		Motor driver:	FAULHABER MCBL 3006C
//		Motor:		FAULHABER MINIMOTOR SA 3654
//		SPI/UART IC:	MCP2515
//Version:	1.1
//Description:	This project is aiming to help a two wheel robot restore balance
//		the they are powered on.
//
//************************************************************************
//		Version		Changes
//		1.0		first version.
//				Can keep the car balance with two wheels.
//		1.1		improved performance in console.	
//		1.2		changed imu and motor directions to fit
//				mcar 2.0 hardware.
//		1.2.1		separated motor.c stop use function in spi.c.
//				fixed some bugs.
//		1.2.2		add options for balance function,which can be operated in
//				config.h. improved simple time control and motor position
//				control performance.
//
//************************************************************************


#include        "common.h"
#include	"main.h"

#define		FILEMCAR

//main program
int main(void)
{
	double imu_data[3];	//imu data buffer used in kalman filter initilization
	int ret=1;		//return value of sys_interface
	pthread_t sys;
	int motor_fd;

	signal(SIGINT,stop);

	sys_welcome();

	file_loadpara("para.cfg",para_num,para);

	imu_fd=imu_init();
	if(imu_fd<0)
	{
		printf("[MAIN]error: imu initilization failed.");
		clean();
	}

//	fir_num=file_loadfir("fir.cfg",fir_para);	//load FIR filter parameters

	imu_rd(imu_fd,imu_data);
	kalman_imu_init(imu_data[1]);	//use accelemeter data to initilize kalman filter's data

	pthread_create(&sys,NULL,sys_interface,NULL);	//start interface process	

	while(sys_run);

	clean();
	return 0;
}
