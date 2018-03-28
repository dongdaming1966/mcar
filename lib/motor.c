#include	"common.h"
#include        <string.h>
#include        <termios.h>

#ifndef		FILEMCP
#include	"mcp.c"
#endif

#define		FILEMOTOR

int motor_init()
{
	int fd;

	fd=mcp_init();

	return fd;
}


int motor_en(int fd,int index)
{	
	spi_transfer(fd,0,SET_TXBUFF(0,0x00,2));
	spi_transfer(fd,0,SET_TXDATA(0,2),0x01,0x00);
	spi_transfer(fd,0,SET_TXSEND(0));

	//fault reset
	while((mcp_chk(fd,0)));

	spi_transfer(fd,0,SET_TXBUFF(0,(0x600+index),8));
	spi_transfer(fd,0,SET_TXDATA(0,8),0x2b,0x40,0x60,0x00,0x80,0x00,0x00,0x00);
	spi_transfer(fd,0,SET_TXSEND(0));

	//shutdown	
	while((mcp_chk(fd,0)));

	spi_transfer(fd,0,SET_TXBUFF(0,(0x600+index),8));
	spi_transfer(fd,0,SET_TXDATA(0,8),0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00);
	spi_transfer(fd,0,SET_TXSEND(0));

	//switch on
	while((mcp_chk(fd,0)));

	spi_transfer(fd,0,SET_TXBUFF(0,(0x600+index),8));
	spi_transfer(fd,0,SET_TXDATA(0,8),0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00);
	spi_transfer(fd,0,SET_TXSEND(0));

	//enable operation
	while((mcp_chk(fd,0)));
	
	spi_transfer(fd,0,SET_TXBUFF(0,(0x600+index),8));
	spi_transfer(fd,0,SET_TXDATA(0,8),0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00);
	spi_transfer(fd,0,SET_TXSEND(0));

	while((mcp_chk(fd,0)));
	
	spi_transfer(fd,0,SET_TXBUFF(0,(0x300+index),5));
	spi_transfer(fd,0,SET_TXDATA(0,5),0xfd,0xff,0xff,0xff,0xff);
	spi_transfer(fd,0,SET_TXSEND(0));

	while((mcp_chk(fd,0)));

	spi_transfer(fd,0,SET_TXBUFF(0,(0x300+index),5));
	spi_transfer(fd,0,SET_TXDATA(0,5),0xb8,0x00,0x00,0x00,0x00);
	spi_transfer(fd,0,SET_TXSEND(0));

	return 0;	
}

int motor_di(int fd,int index)
{
	while((mcp_chk(fd,0)));

	spi_transfer(fd,0,SET_TXBUFF(0,(0x600+index),8));
	spi_transfer(fd,0,SET_TXDATA(0,8),0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00);
	spi_transfer(fd,0,SET_TXSEND(0));

	return 0;
}

int motor_wr_v(int fd,int index,long vel,long limit)
{
	if(vel>limit) vel=limit;
	if(vel<-limit) vel=-limit;

	while((mcp_chk(fd,0)));

	spi_transfer(fd,0,SET_TXBUFF(0,(0x300+index),5));
	spi_transfer(fd,0,SET_TXDATA(0,5),0x93,vel&0xff,(vel>>8)&0xff,(vel>>16)&0xff,(vel>>24)&0xff);
	spi_transfer(fd,0,SET_TXSEND(0));

//	printf("output:%x %x %x %x\n",vel&0xff,(vel>>8)&0xff,(vel>>16)&0xff,(vel>>24)&0xff);
	return 0;
}

int motor_wr_la(int fd,int index,float pos)
{
	long data;

	data=pos*23*3000/360;

	while((mcp_chk(fd,0)));

	spi_transfer(fd,0,SET_TXBUFF(0,(0x300+index),5));
	spi_transfer(fd,0,SET_TXDATA(0,5),0xb4,data&0xff,(data>>8)&0xff,(data>>16)&0xff,(data>>24)&0xff);
	spi_transfer(fd,0,SET_TXSEND(0));



	return 0;
}

int motor_wr_m(int fd,int index)
{
	while((mcp_chk(fd,0)));

	spi_transfer(fd,0,SET_TXBUFF(0,(0x300+index),5));
	spi_transfer(fd,0,SET_TXDATA(0,5),0x3c,0x00,0x00,0x00,0x00);
	spi_transfer(fd,0,SET_TXSEND(0));

	return 0;
}

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
