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
//		1.2.3		added matrix numeration function. rewrite kalman file.
//		1.2.4		added PID with feeedback linearzation controller
//		1.3		rebuild project structure.
//				added MPC controller
//		1.3.1		improved MPC controller performance
//		1.3.2		add position control for mpc
//				can read motor status
//		1.3.3		add mpu6050 support,tested on inertia wheel model
//
//************************************************************************


#include	"main.h"

//main program
int main(void)
{
	pthread_t sys;
	void *sys_ret;

	setbuf(stdout, NULL);	//disable printf buff

	signal(SIGINT,stop);
	sys_welcome();

	file_loadpara("para.cfg",PARAMETERNUMBER,para);

	imu_fd=imu_init();
	if(imu_fd<0)
	{
		printf("[MAIN]error: imu initilization failed.");
		clean();
	}

	pthread_create(&sys,NULL,sys_interface,NULL);	//start interface process	

	pthread_join(sys,&sys_ret);
	clean();

	return 0;
}
