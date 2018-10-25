//File name:	kalman.c

#include	"common.h"
#include	"config.h"

#ifndef		FILEMAT
#include	"matrix.c"
#endif

#define		FILEKALMAN

double a[2][2]={{1,-SAMPLETIME},{0,1}};
double b[2][2]={{SAMPLETIME,0},{0,0}};
double c[2][2]={{1,0},{0,0}};
double p[2][2]={{1,0},{0,1}};
double q[2][2]={{0.001,0},{0,0.001}};
double r[2][2]={{20,0},{0,0}};
double x[2][2]={0};
double y[2][2]={0};
double u[2][2]={0};
double k[2][2]={0};
double unit[2][2]={{1,0},{0,1}};
double _unit[2][2]={{-1,0},{0,-1}};
double temp1[99],temp2[99],temp3[99];

//******************************************
//Name:		kalman
//Parameter:	output	double	model output data
//		input	double	model input data
//Return:	result	*double	kalman filter output
//******************************************
double * kalman(double output,double input)
{
	u[0][0]=input;

	y[0][0]=output;

	//xk=a*x(k-1)+b*u(k-1)
	matrix_multi(2,2,temp1,a,x);
	matrix_multi(2,2,temp2,b,u);
	matrix_sum(2,2,x,temp1,temp2);

	//pk=a*p(k-1)*a'+q
	matrix_trans(2,temp1,a);
	matrix_multi(2,3,temp1,a,p,temp1);
	matrix_sum(2,2,p,temp1,q);

	//kk=pk*c'*(c*pk*c')^(-1)
	matrix_trans(2,temp1,c);
	matrix_multi(2,3,temp2,c,p,temp1);
	matrix_sum(2,2,temp2,temp2,r);
//	matrix_inv(2,temp2,temp2);
	temp2[0]=1/temp2[0];
	matrix_multi(2,3,k,p,temp1,temp2);

	//xk=xk+kk*(y-c*xk)
	matrix_multi(2,3,temp1,_unit,c,x);
	matrix_sum(2,3,temp1,y,temp1,temp2);
	matrix_multi(2,2,temp1,k,temp1);
	matrix_sum(2,2,x,x,temp1);

	//pk=(i-kk*c)*pk
	matrix_multi(2,3,temp1,_unit,k,c);
	matrix_sum(2,2,temp1,unit,temp1);
	matrix_multi(2,2,p,temp1,p);

//	matrix_muilti(2,2,temp1,c,x);
	temp1[0]=x[0][0];
	temp1[1]=input-x[1][0];
	return temp1;
}

