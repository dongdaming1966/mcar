#ifndef SYS_H
#define SYS_H

#include	"common.h"
#include	"motor.h"
#include	"func.h"
#include	"file.h"
#include	<pthread.h>

#define		COMMAXNUM 99
#define		COMMAXLEN 20

extern double para[];
extern int para_num;
extern int sys_run;
extern int balance_run;
extern int cali_run;

void sys_welcome();
void sys_para();
void* sys_interface(void* arg);

#endif //SYS_H
