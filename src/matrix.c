//File name:	matrix.c
//Author:	Dong Daming

#include <matrix.h>
#include <common.h>
#include <stdarg.h>

//******************************************
//Name:		matrix_print
//Parameter:	size	int	size of matrix
//		num	int	number of matrix
//		...	*double	matrix
//Return:	void
//Description:	print the matrixes
//******************************************
void matrix_print(int size,int num, ...)
{
	int i,j;
	double *matrix_input;
	va_list valist;

	va_start(valist,num);
	for(i=0;i<num;i++)
	{
		printf("matrix %d:",i+1);
		matrix_input=va_arg(valist,double *);
		for(j=0;j<size*size;j++)
		{
			if(j%size==0)
				printf("\n");
			printf("%lf ",matrix_input[j]);
		}
		printf("\n");
	}
	printf("\n");
}

//******************************************
//Name:		matrix_changesize
//Parameter:	targ	double* target matrix
//		insize	int	size of input matrix
//		outsize	int	size of output matrix
//		
//Return:	void
//Description:	change matrix size
//******************************************
void matrix_changesize(double* targ, int insize, int outsize)
{
	int i,j;
	double tmp[100];

	if(insize>9||outsize>9)
		printf("[MATRIX]ERROR: Matrix is too large!\n");
	if(insize>=outsize)
		for(i=0;i<outsize;i++)
			for(j=0;j<outsize;j++)
				tmp[i*outsize+j]=*(targ+i*insize+j);
			
	else
	{
		for(i=0;i<insize;i++)
		{
			for(j=0;j<insize;j++)
				tmp[i*outsize+j]=*(targ+i*insize+j);
			for(j=insize;j<outsize;j++)
				tmp[i*outsize+j]=0;
		}
		for(i=insize;i<outsize;i++)
			for(j=0;j<outsize;j++)
				tmp[i*outsize+j]=0;
	}

	for(i=0;i<outsize;i++)
		for(j=0;j<outsize;j++)
			*(targ+i*outsize+j)=tmp[i*outsize+j];

	
}

//******************************************
//Name:		matrix_trans
//Parameter:	size	int	size of matrix
//		...	*double	result
//			*double	target matrix
//Return:		int	if matrix is too large
//				to process.
//Description:	matrix transposition
//******************************************
int matrix_trans(int size, ...)
{
	double tmp;
	int i,j;
	double *output,*input;
	va_list valist;

	if(size>9)
	{
		printf("ERROR: matrix is too large!");
		return -1;
	}

	va_start(valist,size);
	output=va_arg(valist,double *);
	input=va_arg(valist,double *);

	for(i=0;i<size;i++)
	{
		for(j=i+1;j<size;j++)
		{
			tmp=*(input+j+i*size);
			*(output+j+i*size)=*(input+j*size+i);
			*(output+j*size+i)=tmp;
		}
		*(output+i*size+i)=*(input+i*size+i);
	}
	return 0;
}

//******************************************
//Name:		matrix_inv
//Parameter:	size	int	size of matrix
//		...	*double	result
//			*double	target matrix
//Return:		int	if matrix is too large
//				to process.
//Description:	matrix inversion
//******************************************
int matrix_inv(int size, ...)
{
	double tmp[100];
	int i,j,k;
	double *output,*input;
	va_list valist;

	if(size>9)
	{
		printf("ERROR: matrix is too large!");
		return -1;
	}

	va_start(valist,size);
	output=va_arg(valist,double *);
	input=va_arg(valist,double *);

	for(i=0;i<size*size;i++)
		*(tmp+i)=*(input+i);

	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			if(i==j)
				*(output+i*size+j)=1;
			else
				*(output+i*size+j)=0;

	for(i=0;i<size;i++)
	{
		for(j=i;j<size;j++)
		{
			for(k=size-1;k>=0;k--)
				*(output+j*size+k)/=*(tmp+j*size+i);
			for(k=size-1;k>=i;k--)
				*(tmp+j*size+k)/=*(tmp+j*size+i);
		}
		for(j=i+1;j<size;j++)
		{
			for(k=0;k<size;k++)
			{
				*(output+j*size+k)-=*(output+i*size+k);
				*(tmp+j*size+k)-=*(tmp+i*size+k);
			}
		}
	}
	for(i=size-1;i>=0;i--)
	{
		for(j=i-1;j>=0;j--)
		{
			for(k=0;k<size;k++)
				*(output+j*size+k)-=(*(output+i*size+k))*(*(tmp+j*size+i));
			for(k=0;k<size;k++)
				*(tmp+j*size+k)-=(*(tmp+i*size+k))*(*(tmp+j*size+i));
		}
	}

	return 0;
}

//******************************************
//Name:		matrix_sum
//Parameter:	size	int	size of matrix
//		num	int	number of matrix
//		...	*double	matrix
//Return:	void
//Description:	add matrixes
//******************************************
int matrix_sum(int size,int num, ...)
{
	double *input,*output;
	double tmp[100];
	int i,j;
	va_list valist;

	if(size>9)
	{
		printf("ERROR: matrix is too large!");
		return -1;
	}

	va_start(valist,num);
	output=va_arg(valist,double *);
	input=va_arg(valist,double *);
	for(i=0;i<size*size;i++)
		*(tmp+i)=*(input+i);

	for(i=1;i<num;i++)
	{
		input=va_arg(valist,double *);
		for(j=0;j<size*size;j++)
			*(tmp+j)+=*(input+j);
	}

	for(i=0;i<size*size;i++)
		*(output+i)=*(tmp+i);
	return 0;

}

//******************************************
//Name:		matrix_multi
//Parameter:	size	int	size of matrix
//		num	int	number of matrix
//		...	*double	matrix
//Return:	void
//Description:	multiply matrixes
//******************************************
int matrix_multi(int size,int num, ...)
{
	double *input,*output;
	double tmp[100]={0};
	double sum[99]={0};
	int i,j,k;
	va_list valist;

	if(size>9)
	{
		printf("ERROR: matrix is too large!");
		return -1;
	}

	va_start(valist,num);
	output=va_arg(valist,double *);
	input=va_arg(valist,double *);
	for(i=0;i<(size*size);i++)
		*(sum+i)=*(input+i);
	for(;num>1;num--)
	{
		input=va_arg(valist,double *);
		for(j=0;j<size;j++)
		{
			for(i=0;i<size;i++)
			{
				for(k=0;k<size;k++)
					tmp[i+j*size]+=(*(sum+size*j+k))*(*(input+i+size*k));
			}
		}
		for(i=0;i<size*size;i++)
		{
			*(sum+i)=tmp[i];
			tmp[i]=0;
		}
	}
	for(i=0;i<size*size;i++)
	{
		*(output+i)=*(sum+i);
	}
	return 0;
}
