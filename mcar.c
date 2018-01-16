#include        "common.h"
#include	"lib/imu.c"
#include	"lib/motor.c"
#include	"lib/plot.c"
#include	<pthread.h>
#include	<sys/time.h>
#include	<sys/wait.h>

#define IMU_BIAS 0

int V0=0;
int V1=0;
int imu_fd,motor0_fd,motor1_fd;
int16_t imu_data[2];
int16_t motor0_buff[3],motor1_buff[3];
FILE *fd;


void* delay5()
{
	usleep(4000);
}

void* motor0()
{
	motor_write(motor0_fd,V0);
	motor_read(motor0_fd,motor0_buff);
}

void motor1()
{
	motor_write(motor1_fd,V1);
	motor_read(motor1_fd,motor1_buff);
}

void imu()
{
	imu_read(imu_fd,imu_data);
	imu_data[1]-=IMU_BIAS;
}

void stop(int signo)
{
	motor_di(motor0_fd);
	motor_di(motor1_fd);
	fclose(fd);
	plot();
	_exit(0);
}
/*
int main(void)
{
	double kp,kd;
	struct timeval startt,t1,t2;
	pthread_t m0,m1,im,ti;

	signal(SIGINT,stop);

	imu_fd=imu_init();
	motor0_fd=motor_en(0);	
	motor1_fd=motor_en(1);	
	fd=fopen("gnu.dat","w+");
	scanf("%lf,%lf",&kp,&kd);
	gettimeofday(&startt,NULL);
	while(1){
		gettimeofday(&t1,NULL);
		pthread_create(&m0, NULL,motor0,NULL);
		pthread_create(&ti, NULL,delay5,NULL);
//		motor0();
		motor1();
		imu();
		fprintf(fd,"%d\t%d\t%d\n",(startt.tv_sec-t1.tv_sec)*1000000+startt.tv_usec-t1.tv_usec,imu_data[0],imu_data[1]);
    		
//		printf("%d\t%d\t%d\n",(startt.tv_sec-t1.tv_sec)*1000000+startt.tv_usec-t1.tv_usec,V0,motor0_buff[0]);

		pthread_join(m0,NULL);
		pthread_join(ti,NULL);
		gettimeofday(&t2,NULL);
		V0=kp/100*imu_data[1]+kd/100*imu_data[0];
		V1=-V0;
//		printf("Time:%d\n",(t2.tv_sec-t1.tv_sec)*1000000+t2.tv_usec-t1.tv_usec);
		printf("%d\t%d\n",imu_data[0],imu_data[1]);
//		printf("%d %d %x\t",motor1_buff[0],motor1_buff[1],motor1_buff[2]);
//		printf("%d %d %x\n",motor0_buff[0],motor0_buff[1],motor0_buff[2]);
	}
	return 0;
}*/


int main(void)
{
	double kp,kd;
	struct timeval startt,t1,t2;
	pthread_t m0,m1,im,ti;

	signal(SIGINT,stop);

	imu_fd=imu_init();
	fd=fopen("gnu.dat","w+");
	scanf("%lf,%lf",&kp,&kd);
	gettimeofday(&startt,NULL);
	while(1){
		gettimeofday(&t1,NULL);
		imu();
		fprintf(fd,"%d\t%d\t%d\n",(startt.tv_sec-t1.tv_sec)*1000000+startt.tv_usec-t1.tv_usec,imu_data[0],imu_data[1]);
    		

		gettimeofday(&t2,NULL);
		printf("%d\t%d\n",imu_data[0],imu_data[1]);
	}
	return 0;
}
