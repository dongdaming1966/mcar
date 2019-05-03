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
	int fd,i;
	uint8_t receive[2];

	fd=spi_init(0);	
	//set MSC_CTRL register 
	spi_transfer(fd,0,2,0,0xb4,0x06);
	usleep(2000);
	spi_transfer(fd,0,2,0,0xb5,0x00);
	usleep(2000);

	//set SMPL_PRD register
	spi_transfer(fd,0,2,0,0xb6,0x01);
	usleep(2000);
	spi_transfer(fd,0,2,0,0xb7,0x00);
	usleep(2000);

	//set SENS_AVG register
	spi_transfer(fd,0,2,0,0xb8,0x02);
	usleep(2000);
	spi_transfer(fd,0,2,0,0xb9,0x04);
	usleep(2000);

	//set gyro and accelerometer offset registers
	for(i=0x9a;i<0xa6;i++)
	{
		spi_transfer(fd,0,2,0,i,0x00);
		usleep(2000);
	}
	

	//check product id
	spi_transfer(fd,0,2,0,0x56,0x00);
	usleep(2000);

	//check MSC_CTRL
	spi_transfer(fd,1,2,0,receive,0x34,0x00);
	usleep(2000);

	if((receive[0]==0x40)&&(receive[1]==0x15))
		printf("[IMU] product id checked.\n");
	else
	{
		printf("[IMU] error: product id is wrong received %x%x\n",receive[0],receive[1]);
		return -1;
	}

	//check SIMPL_PRD
	spi_transfer(fd,1,2,0,receive,0x36,0x00);
	usleep(2000);

	if((receive[0]==0x00)&&(receive[1]==0x06))
		printf("[IMU] MSC_CTL checked.\n");
	else
	{
		printf("[IMU] error: MSC_CTL is wrong received %x%x\n",receive[0],receive[1]);
		return -1;
	}

	//check SENS_AVG
	spi_transfer(fd,1,2,0,receive,0x38,0x00);
	usleep(2000);

	if((receive[0]==0x00)&&(receive[1]==0x01))
		printf("[IMU] SIMPL_PRD checked.\n");
	else
	{
		printf("[IMU] error: SIMPL_PRD is wrong received %x%x\n",receive[0],receive[1]);
		return -1;
	}

	//read last register
	spi_transfer(fd,1,2,0,receive,0x00,0x00);
	usleep(2000);

	if((receive[0]==0x04)&&(receive[1]==0x02))
		printf("[IMU] SENS_AVG checked.\n");
	else
	{
		printf("[IMU] error: SENS_AVG is wrong received %x%x\n",receive[0],receive[1]);
		return -1;
	}
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
	
	uint8_t com[24]={0x3e,0x00};
	uint8_t buff[24];
	double accy,accz;

	spi_transfer(fd,2,24,0,com,buff);
	
	data[1]=DATACONVERT((int16_t)buff[4]<<8|buff[5])*0.05*PI/180;
	accy=DATACONVERT((int16_t)buff[12]<<8|buff[13])*0.00333;
	accz=DATACONVERT((int16_t)buff[14]<<8|buff[15])*0.00333;

	//SUPPLY OUT	
	spi_transfer(fd,0,2,0,0x02,0x00);
	spi_transfer(fd,1,2,0,buff,0xb4,0x06);
//	data[2]=DATACONVERT((int16_t)buff[0]<<8|buff[1])*2.418;

	data[2]=accy;
	accy-=ACCBIAS;			//make data symmetrical
	if(accy>1)	accy=1;
	if(accy<-1)	accy=-1;

	//Use accelerometer yz axis data to calculate angle in full range.
	if(accz<0)			
		data[0]=-asin(accy);
	else if(accy>0)
		data[0]=-PI+asin(accy);
	else
		data[0]=PI+asin(accy);
	
}
                    
