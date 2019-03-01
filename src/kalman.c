//File name:	kalman.c
//Author:	Dong Daming

#include	"common.h"
#include 	"kalman.h"
#include	"config.h"
#include	"matrix.h"

double x[2][2]={0};
double y[2][2]={0};
double u[2][2]={0};
double k[2][2]={0};
double p[2][2]={{1,0},{0,1}};
double unit[2][2]={{1,0},{0,1}};
double _unit[2][2]={{-1,0},{0,-1}};
double temp1[99],temp2[99],temp3[99];

double sys_x[4][4]={0};
double sys_y[4][4]={0};
double sys_u[4][4]={0};
double sys_k[4][4]={0};
double sys_p[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
double sys_unit[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
double sys__unit[4][4]={{-1,0,0,0},{0,-1,0,0},{0,0,-1,0},{0,0,0,-1}};
double temp4[99],temp5[99],temp6[99];

//******************************************
//Name:		kalman
//Parameter:	output	double	model output data
//		input	double	model input data
//Return:	result	*double	kalman filter output
//******************************************
double * kalman(double output,double input)
{
	double a[2][2]={{1,-SAMPLETIME},{0,1}};
	double b[2][2]={{SAMPLETIME,0},{0,0}};
	double c[2][2]={{1,0},{0,0}};

	double q[2][2]={{0.001,0},{0,0.001}};
	double r[2][2]={{20,0},{0,0}};

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

//******************************************
//Name:		kalman
//Parameter:	output	double	model output data
//		input	double	model input data
//Return:	result	*double	kalman filter output
//******************************************
double * kalman_sys(double output[],double input)
{
	double sys_a[4][4]={{1.003,0.03,-1,-0.01},{0.6128,1.003,0,-1},{0,0,1,0},{0,0,0,1}};
	double sys_b[4][4]={{},{-0.0031,0},};
	double sys_c[4][4]={{1,0,0,0},{0,1,0,0},};

	double sys_q[4][4]={{0.001,0,0,0},{0,0.01,0,0},{0,0,0.01,0},{0,0,0,0.01}};
	double sys_r[4][4]={{100,0,},{0,0.00001,},};

//	matrix_print(4,5,sys_a,sys_b,sys_c,sys_q,sys_r);
	sys_u[0][0]=input;
	sys_y[0][0]=output[0];
	sys_y[1][0]=output[1];

	//xk=a*x(k-1)+b*u(k-1)
	matrix_multi(4,2,temp4,sys_a,sys_x);
	matrix_multi(4,2,temp5,sys_b,sys_u);
	matrix_sum(4,2,sys_x,temp4,temp5);

	//pk=a*p(k-1)*a'+q
	matrix_trans(4,temp4,sys_a);
	matrix_multi(4,3,temp4,sys_a,sys_p,temp4);
	matrix_sum(4,2,sys_p,temp4,sys_q);
//	matrix_print(4,1,sys_p);

	//kk=pk*c'*(c*pk*c')^(-1)
	matrix_trans(4,temp4,sys_c);
	matrix_multi(4,3,temp5,sys_c,sys_p,temp4);
	matrix_sum(4,2,temp5,temp5,sys_r);
//	matrix_print(4,1,temp5);
	matrix_changesize(temp5,4,2);
//	matrix_print(2,1,temp5);
	matrix_inv(2,temp5,temp5);
//	matrix_print(2,1,temp5);
	matrix_changesize(temp5,2,4);
	matrix_multi(4,3,sys_k,sys_p,temp4,temp5);

	//xk=xk+kk*(y-c*xk)
	matrix_multi(4,3,temp4,sys__unit,sys_c,sys_x);
	matrix_sum(4,3,temp4,sys_y,temp4,temp5);
	matrix_multi(4,2,temp4,sys_k,temp4);
	matrix_sum(4,2,sys_x,sys_x,temp4);
//	matrix_print(4,1,sys_x);

	//pk=(i-kk*c)*pk
	matrix_multi(4,3,temp4,sys__unit,sys_k,sys_c);
	matrix_sum(4,2,temp4,sys_unit,temp4);
	matrix_multi(4,2,sys_p,temp4,sys_p);
//	matrix_print(4,1,sys_p);

//	matrix_muilti(2,2,temp1,c,x);
	temp4[0]=sys_x[0][0];
	temp4[1]=sys_x[1][0];
	temp4[2]=sys_x[2][0];
	temp4[3]=sys_x[3][0];

	return temp4;
}

