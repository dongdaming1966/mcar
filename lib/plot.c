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
        fprintf(gp, "plot  \"gnu.dat\" using 1:2 with lines title 'velocity',");
        fprintf(gp, "\"gnu.dat\" using 1:3 with lines title 'angle',");
        fprintf(gp, "\"gnu.dat\" using 1:4 with lines title 'motor velocity'\n");
        fclose(gp);
}
