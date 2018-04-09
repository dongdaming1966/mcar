//Projact Name:	mcar
//Author:	Dong Daming
//Last Edited:	2018/3/30
//Hardware:	Controller:	raspberry pi3
//		IMU:		ADIS 16405
//		Motor driver:	FAULHABER MCBL 3006C
//		Motor:		FAULHABER MINIMOTOR SA 3654
//		SPI/UART IC:	MCP2515
//Version:	1.0
//Description:	This project is aiming to help a two wheel robot restore balance
//		the they are powered on.
//
//************************************************************************
//		Version		Changes
//		1.0		first version.
//				Can keep the car balance with two wheels.
//
//************************************************************************


#include        "common.h"
#include	"main.h"

//#define		DEBUG
#define		MOTOR

#define 	ANGLE_BIAS (-0.37)
#define		MOTOR_I_LIMIT 10000000
#define		STARTPERIOD 3
#define		COMMAXNUM 99
#define		COMMAXLEN 20


int imu_fd,motor_fd;
FILE *plot_fd;
struct timeval startt,t1;
double timenow,timep;

double pid_d=0.3;
double pid_i=0.000001;
double pid_p=10;

//clean the handles when program was closed
void clean(void)
{
	motor_di(motor_fd,1);
	motor_di(motor_fd,2);
	fclose(plot_fd);
	close(motor_fd);
	close(imu_fd);
#ifdef	DEBUG
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
	long V0=0;
	long V0_i=0;

	int i;
	int motor_move=0;

	double gyro_i,angle,angle_goal,angle_error;
	double imu_data[3];
	float kalman_data[2];


	while(1)
	{
		gettimeofday(&t1,NULL);

		timenow=t1.tv_sec-startt.tv_sec+((double)t1.tv_usec-(double)startt.tv_usec)/1000000;
		
		imu_rd(imu_fd,imu_data);

		kalman_filter(imu_data[0],imu_data[1],timenow-timep,kalman_data);

		gyro_i+=(imu_data[0]-kalman_data[1])*(timenow-timep);
		timep=timenow;

		//start smoothly
		if (timenow<STARTPERIOD) V0=timenow/STARTPERIOD*600*kalman_data[0]+2*imu_data[0];
		else 
		{
			//set the different goal in different position
			angle_goal-=pid_i*V0;
			angle_error=kalman_data[0]-angle_goal+ANGLE_BIAS;

			//The output of the controller is motor acceleration,which should controll the current. 
			//Use the velocity control instead of current controll, because the current controll cannot set up by CANbus.
			//So use the current controll if is possible.
			V0+=+pid_p*angle_error+pid_d*(imu_data[0]-kalman_data[1]);
			V0_i+=V0;
			if(V0_i>MOTOR_I_LIMIT) V0_i=MOTOR_I_LIMIT;
			if(V0_i<-MOTOR_I_LIMIT) V0_i=-MOTOR_I_LIMIT;
		}
		
		//Let the sampling frequency is the double times of controlling frequency.
		if(motor_move==0)	motor_move++;
		else
		{
			motor_wr_v(motor_fd,1,V0,200*23);
			motor_wr_v(motor_fd,2,-V0,200*23);
			motor_move=0;
		}

#ifdef	DEBUG
		printf("angle:%-7lf\ttime:%-7lf\tV0:%-7d\tgoal:%-7lf\tV0_i:%-7d\tI:%-7lf\n",kalman_data[0],timenow,V0,angle_goal,V0_i,pid_i*V0_i);
		fprintf(plot_fd,"%lf %lf %lf %lf\n",timenow,gyro_i,imu_data[1],kalman_data[0]);
#endif
	}
}


//main program
int main(void)
{
	double imu_data[3];
	char com[COMMAXNUM][COMMAXLEN]={"help",		//index 0
					"exit"		//index 1
					};
	char input[COMMAXLEN];
	int index;
	pthread_t pth;

	signal(SIGINT,stop);
	printf(" ---.---.---   --------    ------   ---  ---\n");
	printf("|   _   __  \\ /   _____| /   __   \\|   |/  /\n");
	printf("|  | | |  |  |   /      |   /  \\   |      /\n");
	printf("|  | | |  |  |  |       |  |    |  |    _/\n");
	printf("|  | | |  |  |   \\_____ |   \\__/   |   | \n");
	printf("|__| |_|  |__|\\________| \\______/\\_|___|\n");

	imu_fd=imu_init();
	motor_fd=motor_init();

#ifdef	MOTOR
	motor_en(motor_fd,1);
	motor_en(motor_fd,2);
#endif

	usleep(100000);		//wait 100ms for motor enabling

	imu_rd(imu_fd,imu_data);
	kalman_init(imu_data[1]);	//use accelemeter data to initilize kalman filter's data

	plot_fd=fopen("gnu.dat","w+");	//file that recording data
	gettimeofday(&startt,NULL);	//record the starttime
	
	pthread_create(&pth,NULL,balance,NULL);	//start balance process	

	printf("Robot will start smoothly. He can not keep balance by himself. Please help the robot balance for now.\n");
	sleep(STARTPERIOD);
	printf("Start up finished. Robot can be released.\nyou can press help for help information.\n");

	while(1)
	{	
		printf(">>");
		scanf("%s",&input);
		for(index=0;index<COMMAXNUM;index++)
		{
			if(!strcmp(input,com[index]))
				break;
		}

		switch(index)
		{
			case 0:
				printf("index |  name  |  description\n");
				printf("---------------------------------\n");
				printf("  1   |  help  | print this map\n");
				printf("  2   |  exit  | exit the program\n");
				break;
			case 1: 
				clean();
				return 0;
			default: 
				printf("command are not recongized. you can press help to get help information.\n>>");
		}
	}

	pthread_join(pth,NULL);	
	return 0;
}
