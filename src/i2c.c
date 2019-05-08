//File name:	i2c.c	
//Author:	Dong Daming

#include "i2c.h"

//******************************************
//Name:		i2c_init
//Parameter:	addr	uint8_t	i2c slave address
//Return:	fd	int	i2c handle
//Description:	i2c initization
//******************************************
int i2c_init(uint8_t addr)
{
	int fd;

	fd = open(I2C_PATH, O_RDWR);

	if(ioctl(fd,I2C_SLAVE,addr)<0)
	{       
		perror("[I2C]  Can't be initialized.");        
		return -1;     
	}

	return fd; 
}

//******************************************
//Name:		i2c_readreg
//Parameter:	fd	int	i2c handle
//		addr	uint8_t	register address
//Return:	buff	uint8_t	received data
//Description:	read register
//******************************************
uint8_t i2c_readreg(int fd,uint8_t addr)
{
	uint8_t buff;

	write(fd,&addr,1);
	read(fd,&buff,1);

	return buff;
}

//******************************************
//Name:		i2c_writereg
//Parameter:	fd	int	i2c handle
//		addr	uint8_t	register address
//		data	uint8_t	send data
//Return:	void
//Description:	write register
//******************************************
void i2c_writereg(int fd,uint8_t addr,uint8_t data)
{
	uint8_t buff[2];

	buff[0]=addr;
	buff[1]=data;

	write(fd,buff,2);
}
