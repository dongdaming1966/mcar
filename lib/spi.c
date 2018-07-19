//File name:	spi.c	
//Author:	Dong Daming

#include	"common.h"
#include	"config.h"

#include        <linux/spi/spidev.h>
#include	<stdarg.h>		//Variable Argument

#define		FILESPI

#define		dataconver(x) (x&0x2000)? x|0xc000:&0x1fff

//******************************************
//Name:		spi_init
//Parameter:	p	int	which device is going to be initilized
//				0--->SPI0
//				1--->SPI1
//Return:	fd	int	spi handle
//Description:	spi initization
//******************************************
int spi_init(int p)
{
	int fd;
	uint8_t  mode 	= 3;             	    
	uint8_t  bits 	= 8; 
	uint32_t speed 	= 500000;            	     
	uint16_t command=0xbe80;

	if(p==0)	fd = open(SPI0_PATH, O_RDWR);
	if(p==1)	fd = open(SPI1_PATH, O_RDWR);

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

//******************************************
//Name:		spi_transfer
//Parameter:	fd	int 	spi handle
//		mod	int	mode
//				0--->do not receive data.follow the data
//				     need to be sent
//				1--->assign the receive buffer first.then
//				     follow the data need to be sent.
//				2--->assign the send buffer first.then
//				     assign the receive buffer.
//				3--->based on mode 0,added print function.
//				     will print the received data and send
//				     data.
//				4--->assign send buffer only.
//		len	int	length of  data 
//		...	int 	data
//Return:	void
//Description:	transfer spi data
//******************************************

int spi_transfer(int fd, int mod, int len,...)

{      
	va_list valist;
	uint8_t send[99];
	uint8_t receive[99];
	unsigned long receive_adr=(unsigned long)receive;
	unsigned long send_adr=(unsigned long)send;
	int i;

	int* adr;
	
	va_start(valist,len);
	
	if(mod==0||mod==3)	
		for(i=0;i<len;i++)	send[i]=va_arg(valist,int);

	if(mod==1)	
	{
		receive_adr = (unsigned long)va_arg(valist,uint8_t*);
		for(i=0;i<len;i++)	send[i]=va_arg(valist,int);
	}

	if(mod==2)
	{
		send_adr = (unsigned long)va_arg(valist,uint8_t*);
		receive_adr = (unsigned long)va_arg(valist,uint8_t*);
	}

	if(mod==4)
		send_adr = (unsigned long)va_arg(valist,int*);

	struct spi_ioc_transfer trans=         
	{
		trans.tx_buf            = send_adr,    
		trans.rx_buf            = receive_adr,   
		trans.len               = len,                               
		trans.delay_usecs       = 0,                     
	};

	if (ioctl(fd, SPI_IOC_MESSAGE(1), &trans)< 0)
	{       
		perror("SPI: SPI_IOC_MESSAGE Failed");        
		return -1;     
	} 

 	if(mod==3)	
	{
		printf("len: %d send:",len);
		for(i=0;i<len;i++)	printf("%x ",send[i]);	
		printf("rec:");
		for(i=0;i<len;i++)	printf("%x ",receive[i]);	
		printf("\n");
	}

	va_end(valist);

	return 0;
}
 
