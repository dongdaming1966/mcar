#include	"common.h"
#include	"math.h"

#ifndef		FILESPI
#include	"spi.c"
#endif

#define		FILEIMU

#define		ACCBIAS 0.055

#define         DATACONVERT(x) ((int16_t) (((x)&0x2000)? (x)|0xc000:(x)&0x1fff))

int imu_init()
{
	int fd,i;
	uint8_t receive[2];

	fd=spi_init(0);	

	spi_transfer(fd,0,2,0x56,0x00);
	usleep(100);
	spi_transfer(fd,1,2,receive,0xb4,0x06);
	usleep(100);
	spi_transfer(fd,0,2,0xb5,0x00);
	usleep(100);
	spi_transfer(fd,0,2,0xb6,0x01);
	usleep(100);
	spi_transfer(fd,0,2,0xb7,0x00);
	usleep(100);
	spi_transfer(fd,0,2,0xb8,0x02);
	usleep(100);
	spi_transfer(fd,0,2,0xb9,0x40);
	usleep(100);
	for(i=0x9a;i<0xa6;i++)
	{
		spi_transfer(fd,0,2,i,0x00);
		usleep(100);
	}

	if((receive[0]==0x40)&&(receive[1]==0x15))
		printf("IMU initilized.\n");
	else
	{
		printf("IMU error:%x%x\n",receive[0],receive[1]);
		return -1;
	}

	return fd; 
}
 
int imu_rd(int fd,double data[3])
{
	
	uint8_t com[24]={0x3e,0x00};
	uint8_t buff[24];
	double accy,accz;

	spi_transfer(fd,2,24,com,buff);
	

//	uint16_t sen[]={0,1,2,3,4,5,6,7,8,9};
//	printf("%x\t%x\t%x\t%x\n",buff[8],buff[9],buff[12],buff[13]);
//	data[0]=buff[4];
//	data[1]=buff[12];
	data[0]=-DATACONVERT((int16_t)buff[4]<<8|buff[5])*0.05;
	accy=DATACONVERT((int16_t)buff[12]<<8|buff[13])*0.00333;
	accz=DATACONVERT((int16_t)buff[14]<<8|buff[15])*0.00333;

	//SUPPLY OUT	
	spi_transfer(fd,0,2,0x02,0x00);
	spi_transfer(fd,1,2,buff,0xb4,0x06);
	data[2]=DATACONVERT((int16_t)buff[0]<<8|buff[1])*2.418;

	accy-=ACCBIAS;
	if(accy>1)	accy=1;
	if(accy<-1)	accy=-1;

	if(accz>0)
		data[1]=-asin(accy)*90/(PI/2);
	else if(accy>0)
		data[1]=-180+asin(accy)*90/(PI/2);
	else
		data[1]=180+asin(accy)*90/(PI/2);
//	data[1]=accy;
//	printf("%d\t%d\n",data[0],data[1]);
	
	return 0;
}
                    
