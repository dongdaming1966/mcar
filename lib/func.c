#include	"common.h"
#include	"config.h"

#include	<sys/time.h>
#include	<sys/wait.h>
#include	<math.h>
#include	<stdlib.h>

#define		FILEFUNC

#define		MOTOR_I_LIMIT 10000000

extern int sys_run;
extern int balance_run;
extern double para[];

int imu_fd,motor_fd;
FILE *plot_fd;

struct timeval timestart,t1;
double timenow,timep;

double fir_para[9999];
int fir_num;

double abs_double(double data)
{
	if(data<0)	return -data;
	else	return data;
}

//clean the handles when program was closed
void clean(void)
{
	fclose(plot_fd);
	close(imu_fd);
#ifdef	PLOT
	plot();
#endif
}

void stop(int signo)
{
	clean();
	_exit(0);
}

void* balance()
{
	double ctl_output[MAXLOADBUFF]={0};

	int i;
	long motor_output=0;

	double gyro_i,angle;
	double motor_i=0;
	double angle_error=0;
	double imu_data[3];
	float kalman_imu_data[2];

#ifdef	TIMECHECK
	double check_time[99];
#endif

	motor_fd=motor_init();
	if(motor_fd<0)
	{
		printf("[MAIN]error: motor initilization failed.");
		clean();
		_exit(0);
	}

	plot_fd=fopen("gnu.dat","w+");	//file that recording data

#ifdef	MOTOR
	motor_en(motor_fd,1);
	motor_en(motor_fd,2);
#endif

	imu_rd(imu_fd,imu_data);
	while((abs(imu_data[0])>3)||(abs(imu_data[1])>3))
		imu_rd(imu_fd,imu_data);

	printf("\b\b\b[FUNC] target angle reached.\n>> ");
	fflush(stdout);

	gettimeofday(&timestart,NULL);	//record the starttime

	while(balance_run)
	{
#ifdef	TIMECHECK
		gettimeofday(&t1,NULL);
		check_time[0]=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#endif
		imu_rd(imu_fd,imu_data);
		kalman_imu_filter(imu_data[0],imu_data[1],timenow-timep,kalman_imu_data);

#ifdef	TIMECHECK
		gettimeofday(&t1,NULL);
		check_time[1]=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#endif

//		gyro_i+=kalman_imu_data[1]*(timenow-timep);

		//set the different goal in different position
		motor_i+=motor_output;
		angle_error=kalman_imu_data[0]+para[2]*motor_output+para[3]*motor_i+para[6]+para[4]*sin(para[5]*2*PI*timenow);

		//The output of the controller is motor acceleration,which should controll the current. 
		//Use the velocity control instead of current controll, because the current controll cannot set up by CANbus.
		//So use current controll if is possible.
		//pd controller
		ctl_output[0]=para[0]*angle_error+para[1]*kalman_imu_data[1];

		if(timenow<STARTPERIOD)
			ctl_output[0]*=timenow/STARTPERIOD;

#ifdef MONITOR
		printf("time:%-7lf\tkalman:%-10lf\t%-10lf\tcontroller%-10lf\n",timenow,kalman_imu_data[0],kalman_imu_data[1],ctl_output[0]);
#endif	
		//motor_output=filter_fir(fir_num,fir_para,ctl_output);

#ifdef	TIMECHECK
		gettimeofday(&t1,NULL);
		check_time[2]=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#endif
		
//		ctl_output[0]=para[4]*sin(para[5]*2*PI*timenow);
		motor_output+=ctl_output[0];

#ifdef	DATARECORD
		fprintf(plot_fd,"%lf %lf %lf %lf %lf %lf %lf\n",timenow,kalman_imu_data[1],kalman_imu_data[0],ctl_output[0],imu_data[0],imu_data[1],imu_data[2]);
#endif

#ifdef	TIMECHECK
		gettimeofday(&t1,NULL);
		check_time[3]=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#endif

#ifdef MOTOR
		motor_wr_v(motor_fd,1,-motor_output,200*23);
		motor_wr_v(motor_fd,2,motor_output,200*23);
#endif

#ifdef	TIMECHECK
		gettimeofday(&t1,NULL);
		check_time[4]=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#endif
		timep=timenow;
		gettimeofday(&t1,NULL);
		timenow=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#ifdef	REALTIME
		//Sample time control
		while((timenow-timep)<=SAMPLETIME){
			gettimeofday(&t1,NULL);
			timenow=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
		}
#endif

#ifdef TIMECHECK
		if(timenow-timep>1.5*SAMPLETIME){
			printf("[FUNC] WARNING:This control period is longer than setting period %lf:",timenow-timep);
			for(i=1;i<5;i++)
				printf(" %lf",check_time[i]-check_time[i-1]);
			printf("\n");
		}
#endif

	}

#ifdef	MOTOR
	motor_di(motor_fd,1);
	motor_di(motor_fd,2);
	close(motor_fd);
#endif
}

void* input_sweep()
{
	int i=0;
	double amp[6]={2,-2,2,-3,3,0};
	double freq[6]={1,1.5,2,2.5,3,0};

	printf("start sweeping\n");

	for(i=0;i<6;i++)
	{
		printf("amp:%lf\tfreq:%lf\n",amp[i],freq[i]);
		while(timenow-STARTPERIOD<10*(i+1))
		{
			para[4]=amp[i];
			para[5]=freq[i];
		}
	}

	para[4]=0;
	para[5]=0;

	printf("input scaning finished!\n");
}
