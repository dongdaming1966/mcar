#ifndef ADIS16405_H
#define ADIS16405_H

#include	"common.h"
#include	"math.h"
#include	"spi.h"

#define		ACCBIAS 0.055

//convert 14bits into 16bits
#define         DATACONVERT(x) ((int16_t) (((x)&0x2000)? (x)|0xc000:(x)&0x1fff))

int adis16405_init(void);
void adis16405_rd(int fd,double data[3]);

#endif //IMU_H
