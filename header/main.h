#ifndef MAIN_H
#define MAIN_H

#include  	"common.h"
#include	"imu.h"
#include	"motor.h"
#include	"plot.h"
#include	"kalman.h"
#include	"file.h"
#include	"filter.h"
#include	"sys.h"
#include	"func.h"

#include	<pthread.h>
#include	<signal.h>


//*********************************
//
// PD Controller
// proportion [0]
// differetiation [1]
//
// velocity gain [2]
// position gain [3]
//
//*********************************
//
// Swing Signal
// amplitude [4]
// frequency [5]
//
//*********************************
//
// Others
// angle bias [6]
//
//********************************
double para[99];
int para_num=PARAMETERNUMBER;

int sys_run=1;
int balance_run=0;
int cali_run=0;

#endif // MAIN_H
