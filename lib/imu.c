#include	"common.h"
#include	"config.h"

#include        <linux/spi/spidev.h>
#define         dataconvert(x)  (x&0x2000)? x|0xc000:x&0x1fff

int imu_init()
{
	int fd;
	uint8_t  mode 	= 3;             	    
	uint8_t  bits 	= 16; 
	uint32_t speed 	= 1000000;            	     
	uint16_t command=0xbe80;

	if ((fd = open(SPI_PATH, O_RDWR))<0)
	{        
		perror("SPI Error: Can't open device.");       
		return -1;     
	}     

	if((ioctl(fd, SPI_IOC_WR_MODE, 		   &mode )<0)|
	   (ioctl(fd, SPI_IOC_RD_MODE, 		   &mode )<0)|
	   (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD,&bits )<0)|
	   (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD,&bits )<0)|
	   (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)<0)|
	   (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed)<0))
	{       
		perror("SPI: Can't be initialized.");        
		return -1;     
	}
     	write(fd,&command,2);    	

    
 
	return fd; 
}
 
void imu_read(int fd,int16_t data[2])
{
	
	uint16_t burst=0x3e00;
	uint16_t buff[12];

//	uint16_t sen[]={0,1,2,3,4,5,6,7,8,9};
	write(fd, &burst, 2);
	read(fd, buff, 24);
//	printf("%d %d %d %d\n",buff[0],buff[1],buff[2],buff[3]);
//	data[0]=buff&0xf;
//	data[1]=buff[2];
	data[0]=dataconvert(buff[1]);// *.05;
	data[1]=dataconvert(buff[5]);// *3.33;
}
                    
