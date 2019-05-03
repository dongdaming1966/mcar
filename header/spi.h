#ifndef SPI_H
#define SPI_H

#include	"common.h"
#include	"config.h"

#include        <linux/spi/spidev.h>
#include	<stdarg.h>		//Variable Argument

#define		dataconver(x) (x&0x2000)? x|0xc000:&0x1fff


int spi_init(int p);
int spi_transfer(int fd, int mod, int len,int len_z,...);

#endif //SPI_H

