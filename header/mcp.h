#ifndef MCP_H
#define MCP_H

#include	"common.h"
#include	"spi.h"

#define		DATALENMAX 8

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
//TXCHK need spi_transfer working in mode 4
#define		SET_TXCHK(num) 3,INST_READ,ADDR_TXB0CTRL+num*16,0x00,2

int mcp_init(void);
void mcp_print(int fd, int addr, int len);
void mcp_chk(int fd, int num);
void mcp_settxbuff(int fd,int num,int id,int len);
void mcp_setdata(int fd,int num,int len,...);
void mcp_txsend(int fd,int num);

#endif //MCP_H
