//File name:	plot.c	
//Author:	Dong Daming
//Last Edited:	2018/4/1

#include        "common.h"

#define		FILEPLOT

#define 	GNUPLOT "gnuplot -persist"
 
//******************************************
//Name:		plot
//Parameter:	void
//Return:	void
//Description:	read gnu.dat and print the data
//******************************************
void plot(void)
{
        FILE *gp;
        gp = popen(GNUPLOT,"w");
        fprintf(gp, "plot  \"gnu.dat\" using 1:2 with lines title 'Gyro_i',");
        fprintf(gp, "\"gnu.dat\" using 1:3 with lines title 'acc',");
        fprintf(gp, "\"gnu.dat\" using 1:4 with lines title 'Kalman'\n");
        fclose(gp);
}
