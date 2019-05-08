#ifndef I2C_H
#define I2C_H

#include	"common.h"
#include	"config.h"

#include        <linux/i2c-dev.h>

int i2c_init(uint8_t addr);
uint8_t i2c_readreg(int fd,uint8_t addr);
void i2c_writereg(int fd,uint8_t addr,uint8_t data);

#endif //I2C_H

