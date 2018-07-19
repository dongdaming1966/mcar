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

#include	<pthread.h>
#include	<sys/time.h>
#include	<sys/wait.h>
#include	<math.h>

//*********************************
//
// PID Controller
// proportion [0]        
// integration [1]        
// differetiation [2]
//
//*********************************
//
// Swing Signal
// amplitude [3]
// frequency [4]
//
//*********************************
//
// Others
// angle bias [5]
//
//********************************
double para[6];
int para_num=6;
