#ifndef		FILEIMU
#include	"imu.c"
#endif

#ifndef		FILEMOTOR
#include	"motor.c"
#endif

#ifndef		FILEPLOT
#include	"plot.c"
#endif

#ifndef		FILEKALMAN
#include	"kalman.c"
#endif

#ifndef		FILEFILE
#include	"file.c"
#endif

#ifndef		FILEFILTER
#include	"filter.c"
#endif

#ifndef		FILESYS
#include	"sys.c"
#endif

#ifndef		FILEFUNC
#include	"func.c"
#endif

#include	<pthread.h>


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
double para[7];
int para_num=7;

int sys_run=1;
int balance_run=0;
