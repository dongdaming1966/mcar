//File name:	mcp2515.c	
//Author:	Dong Daming
//Hardware:	MCP2515

#include "mcp2515.h"

//******************************************
//Name:		mcp2515_init
//Parameter:	void
//Return:	fd	int	spi handle
//Description:	MCP2515 initization
//******************************************
int mcp2515_init(void)
{
	int fd;

	fd=spi_init(1);

	//enter configuration mode
	spi_transfer(fd,0,1,0,INST_RESET);
	
	//baud rate: 500kHz
//	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CNF1,0x00);
//	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CNF2,0x82);
//	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CNF3,0x02);

	//baud rate: 1MHz
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CNF1,0x00);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CNF2,0x80);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CNF3,0x00);

	//receive buff setting
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXB0CTRL,0x60);

	//receive filter
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXF0SIDH,0x00);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXF0SIDL,0x00);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXF0EID8,0x00);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXF0EID0,0x00);
	
	//receive mask
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXM0SIDH,0x00);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXM0SIDL,0x00);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXM0EID8,0x00);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_RXM0EID0,0x00);

	//interrupt
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CANINTF,0x00);
	
	//back to normal mode
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CANSTAT,0x07);

	return fd;
}

//******************************************
//Name:		mcp2515_print
//Parameter:	fd	int 	spi handle
//		addr	int	first address need to print
//		len	int	number of register to print	
//Return:	void
//Description:	print the register content from addr to addr+len
//******************************************
void mcp2515_print(int fd, int addr, int len)
{
	int i;
	uint8_t buff[3];
	for(i=0;i<len;i++)
	{
		spi_transfer(fd,1,2,1,buff,INST_READ,addr+i);
		printf("%x = %x\n",addr+i,buff[2]);
	}
}

//******************************************
//Name:		mcp_chktx
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to check
//Return:	void	
//Description:	check the data in the assigned has been send
//******************************************
void mcp2515_chktx(int fd, int num)
{
	uint8_t buff[3];
	int i=0;

	do
	{
		spi_transfer(fd,1,2,1,buff,INST_READ,ADDR_CANINTF);
		i++;
		if(i>500)
		{
			printf("[MCP]: chktx time out.\n");
			break;
		}
	}while(!(buff[2]>>(2+num)&0x01));
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CANINTF,buff[2]&!(0x04<<num));

}

//******************************************
//Name:		mcp2515_chkrx
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to check
//Return:	void
//Description:	check the data in the assigned has been send
//******************************************
void mcp2515_chkrx(int fd, int num)
{
	uint8_t buff[3];
	int i=0;

	do
	{
		spi_transfer(fd,1,2,1,buff,INST_READ,ADDR_CANINTF);
		i++;
		if(i>500)
		{
			printf("[MCP]: chkrx time out.\n");
			return;
		}
	}while(!((buff[2]>>num)&0x01));
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CANINTF,buff[2]&!(0x01<<num));

}

//******************************************
//Name:		mcp2515_clrx
//Parameter:	fd	int 	spi handle
//		num	int	clear which buffer's flag 
//Return:	void
//Description:	clear receive buffer full interrupt flag
//******************************************
void mcp2515_clrx(int fd, int num)
{
	uint8_t buff[3];

	spi_transfer(fd,1,2,1,buff,INST_READ,ADDR_CANINTF);
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_CANINTF,buff[2]&!(0x01<<num));
}

//******************************************
//Name:		mcp2515_settxbuff
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to set
//		id	int	data id
//		len	int	length of  data 
//Return:	void
//Description:	set tx buffer setting
//******************************************
void mcp2515_settxbuff(int fd,int num,int id,int len)
{
	spi_transfer(fd,0,6,0,INST_TXBUFF+num*2,(id&0x7f8)>>3,(id&0x07)<<5,0x00,0x00,len);
}

//******************************************
//Name:		mcp2515_setdata
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to set
//		len	int	length of  data 
//		...	int 	data
//Return:	void
//Description:	set the buffer setting and data directly
//******************************************
void mcp2515_setdata(int fd,int num,int len,...)
{
	va_list valist;
	int i;
	uint8_t data[DATALENMAX+1];

	data[0]=INST_TXBUFF+1+num*2;

	va_start(valist,len);
	for(i=0;i<len;i++)	data[i+1]=va_arg(valist,int);
	va_end(valist);

	spi_transfer(fd,4,0,len+1,data);
	
}
//******************************************
//Name:		mcp2515_readbuff
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to set
//		inf[]	int	ID & length
//				inf[0] -> standard identifier
//				inf[1] -> extended identifier enable bit 
//				inf[2] -> extended identifier
//				inf[3] -> data length
//		buff[]	int 	data
//Return:	void
//Description:	set the buffer setting and data directly
//******************************************
void mcp2515_readbuff(int fd,int num,int inf[],uint8_t rbuff[])
{
	int i;
	uint8_t buff[99]={0};

	mcp2515_chkrx(fd,num);
	spi_transfer(fd,1,1,11,buff,INST_RXBUFF);

	inf[0]=(buff[1]<<3)|(buff[2]>>5);
	inf[1]=(buff[2]>>3)&0x01;
	inf[2]=((buff[2]|0x3)<<16)|(buff[3]<<8)|buff[4];
	inf[3]=buff[5];

	for(i=0;i<8;i++)
		rbuff[i]=buff[i+6];

/*	printf("standard identifier: %x\n",inf[0]);
	if(inf[1])	
		printf("extended identifier: %x\n",inf[1]);
	printf("data length: %x\n",inf[3]);
	printf("data:");
	for(i=0;i<inf[3];i++)
		printf(" %x",rbuff[i]);
	printf("\n");*/
}

//******************************************
//Name:		mcp2515_txsend
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to set
//Return:	void
//Description:	send the data in buffer 
//******************************************
void mcp2515_txsend(int fd,int num)
{
	spi_transfer(fd,0,3,0,INST_WRITE,ADDR_TXB0CTRL+num*16,0x0b);
}

//******************************************
//Name:		mcp2515_send
//Parameter:	fd	int 	spi handle
//		num	int	which buffer is going to set
//		id	int	data id
//		len	int	length of  data
//		...	int 	data
//Return:	void
//Description:	send data0x00,0x00,0x00,0x00,0x00 
//		assign len=0x40 to send a remote transmission request
//******************************************
void mcp2515_send(int fd,int num,int id,int len,...)
{
	va_list valist;
	int i;
	uint8_t data[DATALENMAX+1];

	//set id
	mcp2515_settxbuff(fd,num,id,len);

	//set data
	data[0]=INST_TXBUFF+1+num*2;
	va_start(valist,len);
	if(len!=0x40)
		for(i=0;i<len;i++)
			data[i+1]=va_arg(valist,int);
	va_end(valist);
	spi_transfer(fd,4,0,len+1,data);

	//send buff
	mcp2515_txsend(fd,num);

	//check buff
	mcp2515_chktx(fd,num);

}

