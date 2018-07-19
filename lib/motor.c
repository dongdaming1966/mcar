//File name:	motor.c	
//Author:	Dong Daming
//Hardware:	motor--->FAULHABER 3564K
//		driver--->FAULHABER MCBL 3006C

#include	"common.h"
#include        <string.h>
#include        <termios.h>

#ifndef		FILEMCP
#include	"mcp.c"
#endif

#define		FILEMOTOR

//******************************************
//Name:		motor_init
//Parameter:	void
//Return:	fd	int	spi handle
//Description:	motor initization
//******************************************
int motor_init()
{
	int fd;

	fd=mcp_init();

	return fd;
}

//******************************************
//Name:		motor_en
//Parameter:	fd	int	spi handle
//		index	int	motor int
//Return:	void
//Description:	enable the motor
//******************************************
void motor_en(int fd,int index)
{	
	mcp_chk(fd,0);

	//start all remote noeds

	mcp_settxbuff(fd,0,0x00,2);
	mcp_setdata(fd,0,2,0x01,0x00);
	mcp_txsend(fd,0);

	//fault reset

	mcp_chk(fd,0);

	mcp_settxbuff(fd,0,(0x600+index),8);
	mcp_setdata(fd,0,8,0x2b,0x40,0x60,0x00,0x80,0x00,0x00,0x00);
	mcp_txsend(fd,0);

	//shutdown	

	mcp_chk(fd,0);

	mcp_settxbuff(fd,0,(0x600+index),8);
	mcp_setdata(fd,0,8,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00);
	mcp_txsend(fd,0);

	//switch on

	mcp_chk(fd,0);

	mcp_settxbuff(fd,0,(0x600+index),8);
	mcp_setdata(fd,0,8,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00);
	mcp_txsend(fd,0);

	//enable operation

	mcp_chk(fd,0);
	
	mcp_settxbuff(fd,0,(0x600+index),8);
	mcp_setdata(fd,0,8,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00);
	mcp_txsend(fd,0);

	//OPMOD:FAULHABER mode

	mcp_chk(fd,0);
	
	mcp_settxbuff(fd,0,(0x300+index),5);
	mcp_setdata(fd,0,5,0xfd,0xff,0xff,0xff,0xff);
	mcp_txsend(fd,0);

	//define home position

	mcp_chk(fd,0);

	mcp_settxbuff(fd,0,(0x300+index),5);
	mcp_setdata(fd,0,5,0xb8,0x00,0x00,0x00,0x00);
	mcp_txsend(fd,0);
	
}

//******************************************
//Name:		motor_di
//Parameter:	fd	int	spi handle
//		index	int	motor int
//Return:	void
//Description:	disenable the motor
//******************************************
void motor_di(int fd,int index)
{
	mcp_chk(fd,0);

	mcp_settxbuff(fd,0,(0x600+index),8);
	mcp_setdata(fd,0,8,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00);
	mcp_txsend(fd,0);

}

//******************************************
//Name:		motor_wr_v
//Parameter:	fd	int	spi handle
//		index	int	motor int
//		vel	long	motor velocity
//		limit	long	motor velocity limit
//Return:	void
//Description:	motor velocity control
//******************************************

void motor_wr_v(int fd,int index,long vel,long limit)
{
	if(vel>limit) vel=limit;
	if(vel<-limit) vel=-limit;

	mcp_chk(fd,0);

	mcp_settxbuff(fd,0,(0x300+index),5);
	mcp_setdata(fd,0,5,0x93,vel&0xff,(vel>>8)&0xff,(vel>>16)&0xff,(vel>>24)&0xff);
	mcp_txsend(fd,0);
}

//******************************************
//Name:		motor_wr_la
//Parameter:	fd	int	spi handle
//		index	int	motor int
//		pos	long	position
//		limit	long	motor velocity limit
//Return:	void
//Description:	load absolute position
//******************************************
void motor_wr_la(int fd,int index,float pos)
{
	long data;

	data=pos*23*3000/360;

	mcp_chk(fd,0);

	mcp_settxbuff(fd,0,(0x300+index),5);
	mcp_setdata(fd,0,5,0xb4,data&0xff,(data>>8)&0xff,(data>>16)&0xff,(data>>24)&0xff);
	mcp_txsend(fd,0);

}

//******************************************
//Name:		motor_wr_m
//Parameter:	fd	int	spi handle
//		index	int	motor int
//Return:	void
//Description:	activate position control and start positioning	
//******************************************

int motor_wr_m(int fd,int index)
{
	mcp_chk(fd,0);

	mcp_settxbuff(fd,0,(0x300+index),5);
	mcp_setdata(fd,0,5,0x3c,0x00,0x00,0x00,0x00);
	mcp_txsend(fd,0);

	return 0;
}

//not complete yet
void motor_rd(int fd,int8_t buff[3])
{
	char trace=0xc9;
	int fa,i;
	tcflush(fd,TCIOFLUSH);
	fa=write(fd,&trace,1);
	for(i=0;i<2;i++)
	{
		fa=read(fd,&buff[0],1);
		if(fa<1)	i+=99;
	}

}

