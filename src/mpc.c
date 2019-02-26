#include "solver.h"

Vars vars;
Params params;
Workspace work;
Settings settings;

void mpc_init(void)
{

  set_defaults();
  setup_indexing();
	settings.verbose=0;

	params.x_0[0]=0.5;
	params.x_0[1]=0.1;
	params.Q[0]=1;
	params.Q[1]=0;
	params.Q[2]=0;
	params.Q[3]=1;
	params.R[0]=1;
	params.Q_final[0]=0.1;
	params.Q_final[1]=0;
	params.Q_final[2]=0;
	params.Q_final[3]=0.1;
	params.A[0]=1.003;
	params.A[1]=0.03;
	params.A[2]=0.6128;
	params.A[3]=1.003;
	params.B[0]=0;
	params.B[1]=-0.0031;
	params.u_max[0]=10;
	params.S[0]=100;
}

void mpc_update(double x0,double x1)
{
	params.x_0[0]=x0;
	params.x_0[1]=x1;

}
/*
int main(int argc, char **argv)
{
	double i;
	int j;
	long solver_return;

	mpc_init();
	for(i=-3.14/3;i<3.14/4;i+=0.1)
	{
		mpc_update(i,i);
		solver_return=solve();
		printf(" %lf %ld",params.x_0[0],solver_return);
		for(j=0;j<10;j++)
			printf(" %lf",*vars.u[j]);
		printf("\n");
	}
	return 0;
}
 */