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

#include	<pthread.h>
#include	<sys/time.h>
#include	<sys/wait.h>
#include	<math.h>
