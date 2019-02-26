#ifndef MOTOR_H
#define MOTOR_H

#include        <string.h>
#include        <termios.h>
#include	"common.h"
#include	"mcp.h"

int motor_init(void);
void motor_en(int fd,int index);
void motor_di(int fd,int index);
void motor_wr_v(int fd,int index,double vel,long limit);
void motor_wr_hm(int fd, int index);
void motor_wr_la(int fd,int index,float pos);
int motor_wr_m(int fd,int index);

#endif //MOTOR_H
