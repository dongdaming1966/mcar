#include	"common.h"
#include        <string.h>
#include        <termios.h>

#define DEVICE0 "/dev/ttyUSB0"
#define DEVICE1 "/dev/ttyUSB1"

int serial_init(int num)
{
	int fd;

	struct termios options;

	if(num==0)   	fd = open(DEVICE0, O_RDWR|O_NOCTTY|O_NDELAY);
	if(num==1)   	fd = open(DEVICE1, O_RDWR|O_NOCTTY|O_NDELAY);
   	if (fd < 0)	perror("UART: Failed to open the file. \n"); 

   	tcgetattr(fd, &options);
   	cfsetispeed(&options, B115200);	

   	options.c_cflag |= CS8;	
   	options.c_cflag |= (CLOCAL | CREAD);
   	options.c_cflag &= ~CSIZE;
   	options.c_cflag &= ~CRTSCTS;			
   	options.c_cflag &= ~CSTOPB;
   	options.c_iflag |= IGNPAR;

	options.c_oflag &= ~OPOST; 		
   	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
   	options.c_iflag =(IGNBRK|IGNCR);

	options.c_cc[VMIN]=0;
	options.c_cc[VTIME]=1;

	options.c_iflag &= ~(ICRNL | IGNCR);

    	tcsetattr(fd, TCSANOW, &options);
	
if(	fcntl(fd,F_SETFL,0)<0)printf("Failed\n");

	return fd;
	
}

int motor_en(int num)
{
	int fd;
	char trace[4]={200,0,202,255};

	fd=serial_init(num);
	char buff[3]="EN\r";
	write(fd,buff,sizeof(buff));
	write(fd,&trace,sizeof(trace));


	return fd;	
}

void motor_di(int fd)
{
        char buff[3]="DI\r";
        write(fd,buff,sizeof(buff));
	close(fd);
}

void motor_write(int fd,long vel)
{
	char buff[100];
	int len,nByte;

	if(vel>3000) vel=3000;
	if(vel<-3000) vel=-3000;

	sprintf(buff,"V%ld\r",vel);
	len = strlen(buff);
	write(fd,buff,len);
}
void motor_read(int fd,int16_t buff[3])
{
	char trace=0xc9;
	int fa,i;
	tcflush(fd,TCIOFLUSH);
	write(fd,&trace,1);
	//for(i=0;i<3;i++)
		fa=read(fd,&buff[0],2);
//		fa=read(fd,&buff[1],1);
}
