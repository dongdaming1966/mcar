//File name:	load.c
//Author:	Dong Daming
//Last Edited:	2018/4/16

#include	"common.h"

#define		FILELOAD

#define		MAXLOADBUFF 9999

//******************************************
//Name:		load_fir
//Parameter:	path	*char	the file path
//		buff	*double	buffer
//Return:	j	int	the number of loaded parameters 
//Description:	Load FIR filter's parameters
//******************************************
int load_fir(char *path,double *buff)
{
	FILE *fd;	
	int i=0;
	int j=0;

	fd=fopen(path,"r");

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
