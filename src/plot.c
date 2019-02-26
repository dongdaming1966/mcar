//File name:	plot.c	
//Author:	Dong Daming

#include "plot.h"
 
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
        fprintf(gp, "plot  \"gnu.dat\" using 1:2 with lines title 'kalman_1',");
        fprintf(gp, "\"gnu.dat\" using 1:3 with lines title 'kalman_2',");
        fprintf(gp, "\"gnu.dat\" using 1:4 with lines title 'imu_1',");
        fprintf(gp, "\"gnu.dat\" using 1:5 with lines title 'imu_2'\n");
        fclose(gp);
}
