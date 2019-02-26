//File name:	mcp.c	
//Author:	Dong Daming
//Hardware:	MCP2515

#include "mcp.h"

//******************************************
//Name:		mcp_init
//Parameter:	void
//Return:	fd	int	spi handle
//Description:	MCP2515 initization
//******************************************
int mcp_init(void)
{
	int fd;

	fd=spi_init(1);

	spi_transfer(fd,0,1,INST_RESET);	//CANSTAT:Configuration mode
	//set baud rate 500k.due to the crystal oscillator frequency,can not use 1m
	spi_transfer(fd,0,3,INST_WRITE,0x29,0x82);	//CNF2
	spi_transfer(fd,0,3,INST_WRITE,0x28,0x02);	//CNF3
	spi_transfer(fd,0,3,INST_WRITE,0x2a,0x00);	//CNF1

	spi_transfer(fd,0,3,INST_WRITE,0x3f,0x07);	//CANSTAT

	return fd;
}

//******************************************
//Name:		mcp_print
//Parameter:	fd	int 	spi handle
//		addr	int	first address need to print
//		len	int	number of register to print	
//Return:	void
//Description:	print the register content from addr to addr+len
//******************************************
void mcp_print(int fd, int addr, int len)
{
	int i;
	uint8_t buff[3];
	for(i=0;i<len;i++)
	{
		spi_transfer(fd,1,3,buff,INST_READ,addr+i,0x00);
		printf("%x = %x\n",addr+i,buff[2]);
	}
}

//******************************************
//Name:		mcp_chk
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to check
//Return:	chk_flg	int	if the data has been send
//Description:	check the data in the assigned has been send
//******************************************
void mcp_chk(int fd, int num)
{
	uint8_t buff[3];
	int i=0;

	do
	{
		spi_transfer(fd,1,3,buff,INST_READ,ADDR_TXB0CTRL+num*16,0x00);
		i++;
		if(i>500)
		{
			printf("[MCP]: time out.\n");
			break;
		}
	}while((buff[2]>>3&0x01)&&(i<500));

}

//******************************************
//Name:		mcp_settxbuff
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to set
//		id	int	data id
//		len	int	length of  data 
//Return:	void
//Description:	set tx buffer setting
//******************************************
void mcp_settxbuff(int fd,int num,int id,int len)
{
	spi_transfer(fd,0,6,INST_TXBUFF+num*2,(id&0x7f8)>>3,(id&0x07)<<5,0x00,0x00,len);
}

//******************************************
//Name:		mcp_setdata
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to set
//		len	int	length of  data 
//		...	int 	data
//Return:	void
//Description:	set the buffer setting and data directly
//******************************************
void mcp_setdata(int fd,int num,int len,...)
{
	va_list valist;
	int i;
	uint8_t data[DATALENMAX+1];

	data[0]=INST_TXBUFF+1+num*2;

	va_start(valist,len);
	for(i=0;i<len;i++)	data[i+1]=va_arg(valist,int);
	va_end(valist);

	spi_transfer(fd,4,len+1,data);
	
}

//******************************************
//Name:		mcp_txsend
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to set
//Return:	void
//Description:	send the data in buffer 
//******************************************

void mcp_txsend(int fd,int num)
{
	spi_transfer(fd,0,3,INST_WRITE,ADDR_TXB0CTRL+num*16,0x0b);
}