//File name:	filter.c
//Author:	Dong Daming

#include "filter.h"
//******************************************
//Name:		filter_fir
//Parameter:	order	int	FIR filter order
//		para	*double	parameters
//		data	*double	raw data
//Return:	output	double	filter output
//Description:	FIR filter. 
//******************************************
double filter_fir(int order,double *para,double *data)
{
	int i;
	double output=0;

	for(i=1;i<=order+1;i++)
		output+=para[i]*data[i];
	for(;i>0;i--)
		data[i]=data[i-1];

	return output;

}
