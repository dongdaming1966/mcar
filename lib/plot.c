//File name:	plot.c	
//Author:	Dong Daming

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
        fprintf(gp, "plot  \"gnu.dat\" using 1:2 with lines title 'angle_b',");
        fprintf(gp, "\"gnu.dat\" using 1:3 with lines title 'vel_b',");
        fprintf(gp, "\"gnu.dat\" using 1:4 with lines title 'vel_m',");
        fprintf(gp, "\"gnu.dat\" using 1:5 with lines title 'acc_m'\n");
        fclose(gp);
}
