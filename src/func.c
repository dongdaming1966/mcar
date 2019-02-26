//File name:	func.c	
//Author:	Dong Daming
#include	<stdlib.h>
#include	<sys/time.h>
#include	<sys/wait.h>
#include	<math.h>

#include 	"func.h"
#include	"common.h"
#include	"config.h"
#include	"imu.h"
#include	"kalman.h"
#include	"motor.h"
#include	"file.h"

#ifdef MPC
#include	"solver.h"
#include	"mpc.h"
#endif

FILE *plot_fd;

double abs_double(double data)
{
	if(data<0)	return -data;
	else	return data;
}

//clean the handles when program was closed
void clean(void)
{
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

void* balance(void* arg)
{
	double ctl_output[MAXLOADBUFF]={0};

#ifdef TIMECHECK
	int i;
#endif
	double motor_output=0;

	double motor_i=0;
	double angle_error=0;
	double imu_data[3];
	double *kalman_imu_data;
	
	struct timeval timestart,t1;
	double timenow=0;
	double timep=0;

#ifdef	TIMECHECK
	double check_time[99];
#endif

#ifdef	DATARECORD
	plot_fd=fopen("gnu.dat","w+");	//file that recording data
#endif

#ifdef	MOTOR
	motor_fd=motor_init();
	if(motor_fd<0)
	{
		printf("[FUNC]error: motor initilization failed.");
		clean();
		_exit(0);
	}
	motor_en(motor_fd,1);
	motor_en(motor_fd,2);
#endif

	do {
		imu_rd(imu_fd,imu_data);
	} while(((abs(imu_data[0]*100)>1)||(abs(imu_data[1]*100)>5))&&balance_run);

	if(balance_run)
		printf("\b\b\b[FUNC] target angle reached.\n>> ");

	gettimeofday(&timestart,NULL);	//record the starttime
	timenow=0;
	timep=0;

#ifdef MPC
	mpc_init();
#endif

	while(balance_run)
	{

#ifdef	TIMECHECK
		gettimeofday(&t1,NULL);
		check_time[0]=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#endif

		imu_rd(imu_fd,imu_data);
		kalman_imu_data=kalman(imu_data[1],imu_data[0]);

#ifdef	TIMECHECK
		gettimeofday(&t1,NULL);
		check_time[1]=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#endif

		//integrate for  position
		motor_i+=motor_output;

		//The output of the controller is motor acceleration,which should controll the current. 
		//Use the velocity control instead of current controll, because the current controll cannot set up by CANbus.
		//So use current controll if is possible.

#ifdef PID
		angle_error=*(kalman_imu_data)+para[2]*motor_output+para[3]*motor_i+para[6]+para[4]*sin(para[5]*2*PI*timenow);
		ctl_output[0]=para[0]*angle_error+para[1]**(kalman_imu_data+1);
#endif

#ifdef LINEARPID
		angle_error=*(kalman_imu_data)+para[6]+para[4]*sin(para[5]*2*PI*timenow);
		ctl_output[0]=para[7]*angle_error+para[8]**(kalman_imu_data+1);
		ctl_output[0]=ctl_output[0]/cos(kalman_imu_data[0]);
#endif

#ifdef SLIDINGMODE 
		//ctl_output[0]=-para[12]*((kalman_imu_data[0]>0?-1:1)*para[11]*sqrt(1-cos(kalman_imu_data[0]))-kalman_imu_data[1])/cos(kalman_imu_data[0]);
		ctl_output[0]=para[12]*((para[11]*imu_data[1]+imu_data[0])>0?1:-1);
#endif

#ifdef SLIDINGMODEPID
		angle_error=*(kalman_imu_data)+para[6]+para[4]*sin(para[5]*2*PI*timenow);
		ctl_output[0]=para[13]*angle_error+para[14]**(kalman_imu_data+1);
		if(ctl_output[0]>para[15])
			ctl_output[0]=para[15];
		if(ctl_output[0]<-para[15])
			ctl_output[0]=-para[15];
		ctl_output[0]*=para[16];
		ctl_output[0]/=cos(kalman_imu_data[0]);
#endif

#ifdef LINEARLQR
		angle_error=*(kalman_imu_data)+para[6]+para[4]*sin(para[5]*2*PI*timenow);
		ctl_output[0]=-0.9983*angle_error-1.7301**(kalman_imu_data+1);
		ctl_output[1]=ctl_output[0];
//		ctl_output[0]*=para[17];
		ctl_output[0]=(para[18]*(326.7*sin(*(kalman_imu_data))+*(kalman_imu_data+1)**(kalman_imu_data+1)*sin(*(kalman_imu_data)))-para[17]*ctl_output[0])/(0.42+cos(*(kalman_imu_data)))-para[19]*ctl_output[0];
		ctl_output[0]*=para[20];
//		ctl_output[0]=(326*sin(*(kalman_imu_data))+sin(*(kalman_imu_data))*(*(kalman_imu_data+1))*(*(kalman_imu_data+1))+(-3.556-cos(*(kalman_imu_data)))*ctl_output[0])/(0.423+cos(*(kalman_imu_data)))+ctl_output[0];
//		ctl_output[0]*=para[18];
#endif

#ifdef MPC
		mpc_update(kalman_imu_data[0],kalman_imu_data[1]);
		solve();
		ctl_output[0]=para[21]*vars.u_0[0];
#endif
		//limit output
//		if(ctl_output[0]>1)	ctl_output[0]=1;
//		if(ctl_output[0]<-1)	ctl_output[0]=-1;
		//start up smoothly
		if(timenow<STARTPERIOD)
			ctl_output[0]*=timenow/STARTPERIOD;

#ifdef	TIMECHECK
		gettimeofday(&t1,NULL);
		check_time[2]=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
#endif
		
		motor_output+=ctl_output[0]*SAMPLETIME;

#ifdef	DATARECORD
		fprintf(plot_fd,"%-10lf\t%-10lf\t%-10lf\t%-10lf\t%-10lf\n",timenow,*(kalman_imu_data),*(kalman_imu_data+1),imu_data[0],imu_data[1]);
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

#ifdef MONITOR
		printf("time:%-7lf\tkalman:%-10lf\t%-10lf\toutput:%-10lf\n",timenow,*(kalman_imu_data),*(kalman_imu_data+1),ctl_output[0]);
#endif	

#ifdef	REALTIME
		//Sample time control
		while(((timenow-timep)<=SAMPLETIME)&&balance_run){
			gettimeofday(&t1,NULL);
			timenow=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
		}
#endif

#ifdef TIMECHECK
//		if(timenow-timep>1.5*SAMPLETIME){
//			printf("[FUNC] WARNING:This control period is longer than setting period %lf:",timenow-timep);
			for(i=1;i<5;i++)
				printf(" %lf",check_time[i]-check_time[i-1]);
			printf("\n");
//		}
#endif

	}

	motor_output=0;
#ifdef	MOTOR
	motor_di(motor_fd,1);
	motor_di(motor_fd,2);
	close(motor_fd);
#endif

#ifdef	DATARECORD
	fclose(plot_fd);
#endif

	return 0;
}

void* sweep(void* arg)
{
	int i=0;
	double amp[6]={0.02,-0.02,0.02,-0.02,0.02,-0.02};
	double freq[6]={1,1.5,2,2.5,3,4};

	struct timeval timestart,t1;
	double timenow=0;

	printf("\b\b\b[FUNC] start sweeping.\n>> ");

	gettimeofday(&timestart,NULL);	//record the starttime
	gettimeofday(&t1,NULL);
	timenow=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;

	for(i=0;i<6;i++)
	{
		printf("\b\b\b[FUNC] amp:%lf\tfreq:%lf\n>> ",amp[i],freq[i]);
		while(timenow<10*(i+1))
		{
			para[4]=amp[i];
			para[5]=freq[i];
			gettimeofday(&t1,NULL);
			timenow=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
		}
	}

	para[4]=0;
	para[5]=0;

	printf("\b\b\b[FUNC] sweeping finished.\n>> ");

	return 0;
}

void* calibrate_imu(void* arg)
{
	double imu_data[3];
	int i=1;
	double motor_output;
	double motor_position=0;
	double *kalman_imu_data;

	struct timeval timestart,t1;
	double timenow=0;
	double timep=0;
	
	gettimeofday(&timestart,NULL);	//record the starttime

	motor_fd=motor_init();
	plot_fd=fopen("gnu.dat","w+");	//file that recording data

	if(motor_fd<0)
	{
		printf("[FUNC]error: motor initilization failed.");
		clean();
		_exit(0);
	}

#ifdef	MOTOR
	motor_en(motor_fd,2);
#endif

	//move to horizon position
	while(i && cali_run)
	{
		imu_rd(imu_fd,imu_data);

		if(imu_data[1]>0)
		motor_wr_v(motor_fd,2,-0.5,200*23);

		if(imu_data[1]<0)
		motor_wr_v(motor_fd,2,0.5,200*23);

		if(imu_data[1]<0.01 && imu_data[1]>-0.01)
		{
#ifdef	MOTOR
			motor_wr_v(motor_fd,2,0,200*23);
#endif
			printf("\b\b\b[FUNC] Reached horizon position.\n>> ");
			i=0;
		}
	}
	
	
	gettimeofday(&timestart,NULL);	//record the starttime

	while(cali_run)
	{
		gettimeofday(&t1,NULL);
		timenow=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;

		motor_output=2*cos(3*2*PI*timenow);
#ifdef	MOTOR
		motor_wr_v(motor_fd,2,motor_output,200*23);
#endif

		imu_rd(imu_fd,imu_data);
		kalman_imu_data=kalman(imu_data[1],imu_data[0]);

#ifdef MONITOR
		printf("time:%-7lf\timu:%-7lf\t%-7lf\n",timenow,imu_data[0],imu_data[1]);
#endif	

		//record data
		fprintf(plot_fd,"%lf %lf %lf %lf %lf %lf %lf\n",timenow,*(imu_data),*(imu_data+1),motor_output,motor_position,*(kalman_imu_data),*(kalman_imu_data+1));

#ifdef	REALTIME
		//Sample time control
		while((timenow-timep)<SAMPLETIME)
		{
			gettimeofday(&t1,NULL);
			timenow=t1.tv_sec-timestart.tv_sec+((double)t1.tv_usec-(double)timestart.tv_usec)/1000000;
		}
#endif

		motor_position+=motor_output*(timenow-timep);
		timep=timenow;

	}
	motor_di(motor_fd,2);
	close(motor_fd);
	fclose(plot_fd);

	return 0;
}
