//File name:	imu.c	
//Author:	Dong Daming
//Last Edited:	2018/3/30
//Hardware:	ADIS 16405

#include	"common.h"
#include	"math.h"

#ifndef		FILESPI
#include	"spi.c"
#endif

#define		FILEIMU

#define		ACCBIAS 0.055

//convert 14bits into 16bits
#define         DATACONVERT(x) ((int16_t) (((x)&0x2000)? (x)|0xc000:(x)&0x1fff))

//******************************************
//Name:		imu_init
//Parameter:	void
//Return:	fd	int	spi handle
//Description:	IMU initization
//******************************************
int imu_init()
{
	int fd,i;
	uint8_t receive[2];

	fd=spi_init(0);	
	
	//get product id.it should be 4015
	spi_transfer(fd,0,2,0x56,0x00);
	usleep(2000);

	//set MSC_CTRL register 
	spi_transfer(fd,1,2,receive,0xb4,0x06);
	usleep(2000);
	spi_transfer(fd,0,2,0xb5,0x00);
	usleep(2000);

	//set SMPL_PRD register
	spi_transfer(fd,0,2,0xb6,0x01);
	usleep(2000);
	spi_transfer(fd,0,2,0xb7,0x00);
	usleep(2000);

	//set SENS_AVG register
	spi_transfer(fd,0,2,0xb8,0x02);
	usleep(2000);
	spi_transfer(fd,0,2,0xb9,0x04);
	usleep(2000);

	//set gyro and accelerometer offset registers
	for(i=0x9a;i<0xa6;i++)
	{
		spi_transfer(fd,0,2,i,0x00);
		usleep(2000);
	}

	//check if the product id is right
	if((receive[0]==0x40)&&(receive[1]==0x15))
		printf("IMU initilized.\n");
	else
	{
		printf("IMU error:%x%x\n",receive[0],receive[1]);
		return -1;
	}

	return fd; 
}
 
//******************************************
//Name:		imu_rd
//Parameter:	fd	int	spi handle
//		data[3]	double	use to return the data received.
//				data[0]---->gyro data
//				data[1]---->the angle between IMU z axis
//					    and vertical axis
//				data[2]---->IMU power supply measurement
//Return:	void	
//******************************************
void imu_rd(int fd,double data[3])
{
	
	uint8_t com[24]={0x3e,0x00};
	uint8_t buff[24];
	double accy,accz;

	spi_transfer(fd,2,24,com,buff);
	
	data[0]=DATACONVERT((int16_t)buff[4]<<8|buff[5])*0.05;
	accy=DATACONVERT((int16_t)buff[12]<<8|buff[13])*0.00333;
	accz=DATACONVERT((int16_t)buff[14]<<8|buff[15])*0.00333;

	//SUPPLY OUT	
	spi_transfer(fd,0,2,0x02,0x00);
	spi_transfer(fd,1,2,buff,0xb4,0x06);
	data[2]=DATACONVERT((int16_t)buff[0]<<8|buff[1])*2.418;

	accy-=ACCBIAS;			//make data symmetrical
	if(accy>1)	accy=1;
	if(accy<-1)	accy=-1;

	//Use accelerometer yz axis data to calculate angle in full range.
	if(accz<0)			
		data[1]=-asin(accy)*90/(PI/2);
	else if(accy>0)
		data[1]=-180+asin(accy)*90/(PI/2);
	else
		data[1]=180+asin(accy)*90/(PI/2);
	
}
                    
