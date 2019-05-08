#ifndef MCP_H
#define MCP_H

#include	"common.h"
#include	"spi.h"

#define		DATALENMAX 	8

#define		INST_RESET 	0xC0
#define		INST_READ 	0x03
#define		INST_RXBUFF 	0x90
#define		INST_WRITE 	0x02
#define		INST_TXBUFF 	0x40
#define		INST_RTS 	0x80


#define		ADDR_RXF0SIDH	0x00
#define		ADDR_RXF0SIDL	0x01
#define		ADDR_RXF0EID8	0x02
#define		ADDR_RXF0EID0	0x03
#define		ADDR_RXM0SIDH	0x20
#define		ADDR_RXM0SIDL	0x21
#define		ADDR_RXM0EID8	0x22
#define		ADDR_RXM0EID0	0x23
#define		ADDR_CNF3	0x28
#define		ADDR_CNF2	0x29
#define		ADDR_CNF1	0x2a
#define		ADDR_CANINTF 	0x2c
#define		ADDR_TXB0CTRL 	0x30
#define		ADDR_TXB0SIDH 	0x31
#define		ADDR_TXB0SIDL 	0x32
#define		ADDR_TXB0DLC 	0x35
#define		ADDR_TXB0D0 	0x36
#define		ADDR_CANSTAT	0x3f
#define		ADDR_RXB0CTRL	0x60

#define		SET_TXBUFF(num,id,len) 6,INST_TXBUFF+num*2,(id&0x7f8)>>3,(id&0x07)<<5,0x00,0x00,len
#define		SET_TXDATA(num,len) len+1,INST_TXBUFF+1+num*2
#define		SET_TXSEND(num) 3,INST_WRITE,ADDR_TXB0CTRL+num*16,0x0b
//TXCHK need spi_transfer working in mode 4
#define		SET_TXCHK(num) 3,INST_READ,ADDR_TXB0CTRL+num*16,0x00,2

int mcp2515_init(void);
void mcp2515_print(int fd, int addr, int len);
void mcp2515_chktx(int fd, int num);
void mcp2515_chkrx(int fd, int num);
void mcp2515_clrx(int fd, int num);
void mcp2515_settxbuff(int fd,int num,int id,int len);
void mcp2515_setdata(int fd,int num,int len,...);
void mcp2515_readbuff(int fd,int num,int inf[],uint8_t buff[]);
void mcp2515_txsend(int fd,int num);
void mcp2515_send(int fd,int num,int id,int len,...);

#endif //MCP_H
