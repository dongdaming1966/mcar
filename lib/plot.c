#include        "common.h"

#define GNUPLOT "gnuplot -persist"
 
void plot(void)
{
        FILE *gp;
        gp = popen(GNUPLOT,"w");
        fprintf(gp, "plot  \"gnu.dat\" using 1:2 title 'data1',");
        fprintf(gp, "\"gnu.dat\" using 1:3 title 'data2'\n");
        fclose(gp);
}
