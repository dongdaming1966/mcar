#include        "common.h"

#ifndef		FILEIMU
#include	"lib/imu.c"
#endif

#ifndef		FILEMOTOR
#include	"lib/motor.c"
#endif

#ifndef		FILEPLOT
#include	"lib/plot.c"
#endif

#ifndef		FILEKALMAN
#include	"lib/kalman.c"
#endif

#include	<pthread.h>
#include	<sys/time.h>
#include	<sys/wait.h>
#include	<math.h>

#define 	ANGLE_BIAS (-0.5)
#define		MOTOR_I_LIMIT 1000000

long V0[4]={0};
long V0_filter=0;
long V0_i=0;
float V1=0;
int imu_fd,motor_fd;
double imu_data[3];
float kalman_data[2];
int8_t motor0_buff[3],motor1_buff[3];
double pid_i=0.00000;
FILE *fd;

void stop(int signo)
{
	motor_di(motor_fd,1);
	motor_di(motor_fd,2);
	fclose(fd);
	close(motor_fd);
	close(imu_fd);
//	plot();
	_exit(0);
}


int main(void)
{
	struct timeval startt,t1,t2;
	double timenow,timep;

	int i;

	double gyro_i,angle,angle_goal,angle_error;
	double pid_d=2;
	double pid_p=600;

	pthread_t m0,m1,im,ti;

	uint8_t receive[99];

	signal(SIGINT,stop);

	imu_fd=imu_init();
	motor_fd=motor_init();

	motor_en(motor_fd,1);
	motor_en(motor_fd,2);

	usleep(100000);		//wait 100ms for motor enabling

	imu_rd(imu_fd,imu_data);
	kalman_init(imu_data[1]);

	fd=fopen("gnu.dat","w+");
	gettimeofday(&startt,NULL);
	while(1){
		gettimeofday(&t1,NULL);

		timenow=t1.tv_sec-startt.tv_sec+((double)t1.tv_usec-(double)startt.tv_usec)/1000000;
		
		imu_rd(imu_fd,imu_data);

		kalman_filter(imu_data[0],imu_data[1],timenow-timep,kalman_data);

		gyro_i+=(imu_data[0]+kalman_data[1])*(timenow-timep);
		timep=timenow;

		for(i=2;i>0;i--) V0[i+1]=V0[i];

		//start smoothly
		if (timenow<3) V0[0]=timenow/3*600*kalman_data[0]+2*imu_data[0];
		else 
		{
			angle_goal-=pid_i*V0[1];
			angle_error=kalman_data[0];///-angle_goal;
			pid_p=950-400/(((angle_error>0)?(angle_error/4):(-angle_error/4))+1);
			pid_d=-5.5+7/(((angle_error>0)?(angle_error):(-angle_error))+1);
			V0[0]=V0[1]+pid_p*angle_error+pid_d*(imu_data[0]-kalman_data[1]);//+((pid_i*V0_i)>1000?1000:pid_i*V0_i);
			V0_i+=V0[0];
			if(V0_i>MOTOR_I_LIMIT) V0_i=MOTOR_I_LIMIT;
			if(V0_i<-MOTOR_I_LIMIT) V0_i=-MOTOR_I_LIMIT;
		}
		V0_filter=0.7*V0[0]+0.2*V0[1]+0.05*V0[2]+0.05*V0[3];
		printf("V0:%-7lf\t");
//		printf("angle:%-7lf\ttime:%-7lf\tV0_i:%-7d\tgoal:%-7lf\tkp:%-7lf\tkd:%-7lf\n",kalman_data[0],timenow,V0[1],angle_goal,pid_p,pid_d);

//		V0+=10;
//		if(V0>=23*1500)	V0=23*1500;

//		n=n+1;
//		spi_transfer(imu_fd,imu_data,2,0x56,0x00);
//		printf("imu:%-7lf\t%-7lf\tV0:%lf\n",gyro_i,imu_data[1],angle);

		motor_wr_v(motor_fd,1,V0_filter,100*23);
		motor_wr_v(motor_fd,2,-V0_filter,100*23);

//		motor_wr_la(motor_fd,1,0);
//		motor_wr_la(motor_fd,2,V0);
//		usleep(500);
//		motor_wr_m(motor_fd,1);
//		motor_wr_m(motor_fd,2);
//		usleep(500);

//		fprintf(fd,"%lf %lf %lf %lf\n",timenow,gyro_i,imu_data[1],kalman_data[0]);
    		

//		gettimeofday(&t2,NULL);
	}
	return 0;
}
