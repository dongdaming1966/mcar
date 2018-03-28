#include	"common.h"

#ifndef		FILESPI
#include	"spi.c"
#endif

#define		FILEMCP

#define		INST_RESET 0xC0
#define		INST_READ 0x03
#define		INST_RXBUFF 0x90
#define		INST_WRITE 0x02
#define		INST_TXBUFF 0x40

#define		ADDR_TXB0CTRL 0x30
#define		ADDR_TXB0SIDH 0x31
#define		ADDR_TXB0SIDL 0x32
#define		ADDR_TXB0DLC 0x35
#define		ADDR_TXB0D0 0x36

#define		SET_TXBUFF(num,id,len) 6,INST_TXBUFF+num*2,(id&0x7f8)>>3,(id&0x07)<<5,0x00,0x00,len
#define		SET_TXDATA(num,len) len+1,INST_TXBUFF+1+num*2
#define		SET_TXSEND(num) 3,INST_WRITE,ADDR_TXB0CTRL+num*16,0x0b
//TXCHK need spi_transfer working in moddde 4
#define		SET_TXCHK(num) 3,INST_READ,ADDR_TXB0CTRL+num*16,0x00,2


int mcp_init(void)
{
	int fd;

	fd=spi_init(1);

	spi_transfer(fd,0,3,INST_WRITE,0x3f,0x87);	//CANSTAT:Configuration mode
	spi_transfer(fd,0,3,INST_WRITE,0x2a,0x00);	//CNF1
	spi_transfer(fd,0,3,INST_WRITE,0x29,0x82);	//CNF2
	spi_transfer(fd,0,3,INST_WRITE,0x28,0x02);	//CNF3

	spi_transfer(fd,0,3,INST_WRITE,0x3f,0x07);	//CANSTAT

	return fd;
}

int mcp_print(int fd, int addr, int len)
{
	int i;
	uint8_t buff[3];
	for(i=0;i<len;i++)
	{
		spi_transfer(fd,1,3,buff,INST_READ,addr+i,0x00);
		printf("%x = %x\n",addr+i,buff[2]);
	}

	return 0;
}

int mcp_chk(int fd, int num)
{
	uint8_t buff[3];

	spi_transfer(fd,1,3,buff,INST_READ,ADDR_TXB0CTRL+num*16,0x00);

	return (buff[2]>>3)&0x01;
}

int mcp_setdata(int fd,int addr,int len,...)
{
	va_list valist;
	int i,p;

	va_start(valist,len);

	spi_transfer(fd,0,3,INST_WRITE,ADDR_TXB0SIDH,0x01);	//SIDH
	spi_transfer(fd,0,3,INST_WRITE,ADDR_TXB0SIDL,0x40);	//SIDL
	spi_transfer(fd,0,3,INST_WRITE,ADDR_TXB0DLC,0x01);	//DLC
	spi_transfer(fd,0,3,INST_WRITE,ADDR_TXB0D0,0xff);		//D0
	spi_transfer(fd,0,3,INST_WRITE,ADDR_TXB0CTRL,0x0b);	//TXB0CTRL

//	printf("addr:%d\nlen: %d\nprint: ",addr,len);
	for(i=0;i<len;i++)
	{
		p=va_arg(valist,int);
	//	printf("%d ",p);
	}
//	printf("\n");
	return 0;
}
/*
int main()
{
	int fd,rev;
	
	fd=mcp_init();	

	mcp_transfer(fd,SET_TXBUFF(0,0x301,5));
	mcp_transfer(fd,SET_TXDATA(0,5),0x0f,0x00,0x00,0x00,0x00);
	mcp_transfer(fd,SET_TXSEND(0));

	mcp_print(fd,0x30,16);

	return 0;
}*/
