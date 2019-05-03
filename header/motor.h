#ifndef MOTOR_H
#define MOTOR_H

#include        <string.h>
#include        <termios.h>
#include	"common.h"
#include	"mcp.h"

//get actual position
#define	FAULHABER_POS	0x40
//get target position
#define	FAULHABER_TPOS	0x4b
//get target velocity
#define	FAULHABER_GV	0x3a
//get actual velocity
#define	FAULHABER_GN	0x2b
//get current limit
#define	FAULHABER_GCL	0x19
//get real current
#define	FAULHABER_GRL	0x34

int motor_init(void);
void motor_en(int fd,int index);
void motor_di(int fd,int index);
void motor_wr_v(int fd,int index,double vel,long limit);
void motor_wr_hm(int fd, int index);
void motor_wr_la(int fd,int index,float pos);
void motor_wr_m(int fd,int index);
int32_t motor_rd_raw(int fd,int index,int command);
double motor_rd(int fd,int index,int command);

#endif //MOTOR_H
