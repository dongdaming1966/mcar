//File name:	file.c
//Author:	Dong Daming

#include	"file.h"
#include	"common.h"

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
	if(fd<0)	printf("[FILE] error: FIR setting file lost.\n");

	while(i<10&&j<MAXLOADBUFF-1)
	{
		fscanf(fd,"%lf",&buff[j]);

		if(buff[j]==0)
			i++;
		else
			i=0;
		j++;
	}

	j-=11;
	printf("[FILE] loaded %d FIR parameters.%lf\n",j,buff[j]);

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
	if(fd<0)	printf("[FILE] error: parameters storing file lost.\n");
	else
	{
		printf("[FILE] load parameters:");
		for(i=0;i<num;i++)
		{
			fscanf(fd,"%lf",&buff[i]);
			printf(" %lf",buff[i]);
		}
		printf("\n");
	}

	fclose(fd);

}

//******************************************
//Name:		file_savepara
//Parameter:	path	*char	the file path
//		num	int	number of parameters
//		buff	*double	buffer
//Return:	void
//Description:	Save parameters
//******************************************
void file_savepara(char *path,int num,double *buff)
{
	FILE *fd;
	int i;

	fd=fopen(path,"w");
	if(fd<0)	printf("[FILE] error: failed to open parameters storing file.\n");
	else
	{
		printf("[FILE] save parameters:");
		for(i=0;i<num;i++)
		{
			fprintf(fd,"%lf ",buff[i]);
			printf(" %lf",buff[i]);
		}
		printf("\n");
	}

	fclose(fd);

}
