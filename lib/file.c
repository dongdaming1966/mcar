//File name:	FILE.c
//Author:	Dong Daming
//Last Edited:	2018/4/16

#include	"common.h"

#define		FILEFILE

#define		MAXLOADBUFF 9999

//******************************************
//Name:		file_loadfir
//Parameter:	path	*char	the file path
//		buff	*double	buffer
//Return:	j	int	the number of loaded parameters 
//Description:	Load FIR filter's parameters
//******************************************
int file_loadfir(char *path,double *buff)
{
	FILE *fd;	
	int i=0;
	int j=0;

	fd=fopen(path,"r");
	if(fd<0)	printf("[LOAD]:FIR setting file lost.\n");

	while(i<10&&j<MAXLOADBUFF-1)
	{
		fscanf(fd,"%lf",&buff[j]);
		
	//	printf("%lf\n",buff[j]);

		if(buff[j]==0)
			i++;
		else
			i=0;
		j++;
	}

	j-=11;
	printf("loaded %d FIR parameters.%lf\n",j,buff[j]);

	return j;

}

//******************************************
//Name:		file_loadpara
//Parameter:	path	*char	the file path
//		num	int	number of parameters
//		buff	*double	buffer
//Return:	void
//Description:	Load parameters
//******************************************
void file_loadpara(char *path,int num,double *buff)
{
	FILE *fd;	
	int i;

	fd=fopen(path,"r");
	if(fd<0)	printf("[FILE]:parameters storing file lost.\n");
	else
	{
		for(i=0;i<=num;i++)	fscanf(fd,"%lf",&buff[i]);
		printf("[FILE]:parameters loaded");
	}

}
