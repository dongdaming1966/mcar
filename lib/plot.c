#include        "common.h"

#define		FILEPLOT

#define 	GNUPLOT "gnuplot -persist"
 
void plot(void)
{
        FILE *gp;
        gp = popen(GNUPLOT,"w");
//	fprintf(gp, "set xlable \"Time(second)\"\n");
//	fprintf(gp, "set ylable \"Angle(degree)/angular velocity(degree per second)\"\n");
        fprintf(gp, "plot  \"gnu.dat\" using 1:2 with lines title 'Gyro_i',");
//        fprintf(gp, "\"gnu.dat\" using 1:3 with lines title 'Gyro_i',");
        fprintf(gp, "\"gnu.dat\" using 1:3 with lines title 'acc',");
        fprintf(gp, "\"gnu.dat\" using 1:4 with lines title 'Kalman'\n");
//        fprintf(gp, "\"gnu.dat\" using 1:6 with lines title 'Motor'\n");
        fclose(gp);
}
