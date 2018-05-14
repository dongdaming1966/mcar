//Projact Name:	mcar
//Author:	Dong Daming
//Last Edited:	2018/5/14
//Hardware:	Controller:	raspberry pi3
//		IMU:		ADIS 16405
//		Motor driver:	FAULHABER MCBL 3006C
//		Motor:		FAULHABER MINIMOTOR SA 3654
//		SPI/UART IC:	MCP2515
//Version:	1.1
//Description:	This project is aiming to help a two wheel robot restore balance
//		the they are powered on.
//
//************************************************************************
//		Version		Changes
//		1.0		first version.
//				Can keep the car balance with two wheels.
//		1.1		improved performance in console.	
//
//************************************************************************


#include        "common.h"
#include	"main.h"

#define		FILEMCAR

//#define		DEBUG
//#define		PLOT
#define		MOTOR

#define 	ANGLE_BIAS (-0.37)
#define		MOTOR_I_LIMIT 10000000
#define		STARTPERIOD 3

#define		SAMPLETIME 0.002


int imu_fd,motor_fd;
int balance_run=1;
FILE *plot_fd;
struct timeval timestart,t1;
double timenow,timep;

double pid_p=10;
double pid_i=-0.000001;
double pid_d=0.5;

double swp_amp=0;
double swp_freq=0;

double fir_para[9999];
int fir_num;

//clean the handles when program was closed
void clean(void)
{
	balance_run=0;
	usleep(SAMPLETIME*2*1000000);
#ifdef	MOTOR
	motor_di(motor_fd,1);
	motor_di(motor_fd,2);
	close(motor_fd);
#endif
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
	double angle_goal=0;
	double angle_error=0;
	double imu_data[3];
	float kalman_data[2];


	while(balance_run)
	{
		//Sample time control
		while((timenow-timep)<=SAMPLETIME)
		{
			gettimeofday(&t1,NULL);
			timenow=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
		}
#ifdef DEBUG
		if(timenow-timep>1.5*SAMPLETIME)
			printf("WARNING:This control period is longer than setting period\n");
#endif
		imu_rd(imu_fd,imu_data);

		kalman_filter(imu_data[0],imu_data[1],timenow-timep,kalman_data);

		gyro_i+=(imu_data[0]-kalman_data[1])*(timenow-timep);
		timep=timenow;

		//start smoothly
		if (timenow<STARTPERIOD) ctl_output[0]=timenow/STARTPERIOD*pid_p*kalman_data[0]+pid_d*(imu_data[0]-kalman_data[1]);
		else 
		{
			//set the different goal in different position
			angle_goal+=pid_i*ctl_output[0];
			angle_error=kalman_data[0]-angle_goal+ANGLE_BIAS+swp_amp*sin(swp_freq*2*PI*timenow);

			//The output of the controller is motor acceleration,which should controll the current. 
			//Use the velocity control instead of current controll, because the current controll cannot set up by CANbus.
			//So use the current controll if is possible.
			//pid controller
			ctl_output[0]=pid_p*angle_error+pid_d*(imu_data[0]-kalman_data[1]);
#ifdef DEBUG
			printf("time:%-7lf\toutput:%-7lf %-7lf %-7d\n",timenow,swp_amp,swp_freq,motor_output);
#endif

#ifdef PLOT
			fprintf(plot_fd,"%lf %lf %lf %d %lf %lf %lf\n",timenow,imu_data[0]-kalman_data[1],kalman_data[0],motor_output,ctl_output[0],imu_data[0],imu_data[1]);
#endif
		}
		
		//motor_output=filter_fir(fir_num,fir_para,ctl_output);
		motor_output+=ctl_output[0];

		motor_wr_v(motor_fd,1,motor_output,200*23);
		motor_wr_v(motor_fd,2,-motor_output,200*23);



	}
}

void* input_sweep()
{
	int i=0;
	double amp[6]={1,1,2,3,2,1};
	double freq[6]={0.8,1.5,1.5,1.5,2,2};

	printf("start sweeping\n");

	for(i=0;i<6;i++)
	{
		printf("amp:%lf\tfreq:%lf\n",amp[i],freq[i]);
		while(timenow-STARTPERIOD<10*(i+1))
		{
			swp_amp=amp[i];
			swp_freq=freq[i];
		}
	}

	swp_amp=0;
	swp_freq=0;

	printf("input scaning finished!\n");
}

//main program
int main(void)
{
	double imu_data[3];	//imu data buffer used in kalman filter initilization
	int ret=1;		//return value of sys_interface
	pthread_t pth,swp;

	signal(SIGINT,stop);

	sys_welcome();

	imu_fd=imu_init();
	motor_fd=motor_init();

#ifdef	MOTOR
	motor_en(motor_fd,1);
	motor_en(motor_fd,2);
#endif

//	fir_num=load_fir("fir.cfg",fir_para);	//load FIR filter parameters

	usleep(100000);		//wait 100ms for motor enabling

	imu_rd(imu_fd,imu_data);
	kalman_init(imu_data[1]);	//use accelemeter data to initilize kalman filter's data

	plot_fd=fopen("gnu.dat","w+");	//file that recording data
	gettimeofday(&timestart,NULL);	//record the starttime
	
	pthread_create(&pth,NULL,balance,NULL);	//start balance process	

	printf("Robot will start smoothly. He can not keep balance by himself. Please help the robot balance for now.\n");
	sleep(STARTPERIOD);
	printf("Start up finished. Robot can be released.\nyou can press help for help information.\n");

//	pthread_create(&swp,NULL,input_sweep,NULL);	

	while(ret)
	{	
		ret=sys_interface();		
	}

	clean();
	return 0;
}
