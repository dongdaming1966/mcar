#include "mpc.h"
#include "config.h"

Vars vars; Params params; Workspace work; Settings settings;

void mpc_init(void)
{
	int i;

        set_defaults();
        setup_indexing();
	settings.verbose=0;

	//static state
	//Q[0]=300 R[0]=6 Q_final[0]=100
	params.x_0[0]=0;
	params.x_0[1]=0;
	params.x_0[2]=0;
	params.x_0[3]=0;
	params.Q[0]=300;
	params.Q[1]=0;
	params.Q[2]=0;
	params.Q[3]=0;
	params.Q[4]=0;
	params.Q[5]=0;
	params.Q[6]=0;
	params.Q[7]=0;
	params.Q[8]=0;
	params.Q[9]=0;
	params.Q[10]=0;
	params.Q[11]=0;
	params.Q[12]=0;
	params.Q[13]=0;
	params.Q[14]=0;
	params.Q[15]=0;
	params.R[0]=1;
	params.Q_final[0]=100;
	params.Q_final[1]=0;
	params.Q_final[2]=0;
	params.Q_final[3]=0;
	params.Q_final[4]=0;
	params.Q_final[5]=0;
	params.Q_final[6]=0;
	params.Q_final[7]=0;
	params.Q_final[8]=0;
	params.Q_final[9]=0;
	params.Q_final[10]=0;
	params.Q_final[11]=0;
	params.Q_final[12]=0;
	params.Q_final[13]=0;
	params.Q_final[14]=0;
	params.Q_final[15]=1;
	params.A[0]=1;
	params.A[1]=0.01;
	params.A[2]=0;
	params.A[3]=0;
	params.A[4]=0.3;
	params.A[5]=1;
	params.A[6]=0;
	params.A[7]=0;
	params.A[8]=0;
	params.A[9]=0;
	params.A[10]=1;
	params.A[11]=SAMPLETIME;
	params.A[12]=0;
	params.A[13]=0;
	params.A[14]=0;
	params.A[15]=1;
	params.B[0]=0;
	params.B[1]=-0.0012;
	params.B[2]=0;
	params.B[3]=-SAMPLETIME;
	params.u_max[0]=1000;
	params.S[0]=1000;

	for(i=0;i<22;i++)
	{
		*params.r[i]=0;
		*(params.r[i]+1)=0;
	}


}

void mpc_update(double x0,double x1,double x2,double x3,double slop,double a21,double b21)
{
//	int i;

//	params.A[4]=a21;
//	params.B[1]=b21;

	params.x_0[0]=x0;
	params.x_0[1]=x1;
	params.x_0[2]=x2;
	params.x_0[3]=x3;

	params.r_0[0]=x0;
	params.r_0[1]=-slop*x0;
	params.r_0[2]=-slop;

/*	for(i=1;i<22;i++)
	{
		*params.r[i]=*params.r[i-1]+SAMPLETIME**(params.r[i-1]+1);
		*(params.r[i]+1)=-slop**params.r[i];
	}*/

	params.R[0]=0.5-1.5*x0;
}
