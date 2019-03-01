#ifndef MATRIX_H
#define MATRIX_H

void matrix_print(int size,int num, ...);
void matrix_changesize(double* targ, int insize, int outsize);
int matrix_trans(int size, ...);
int matrix_inv(int size, ...);
int matrix_sum(int size,int num, ...);
int matrix_multi(int size,int num, ...);

#endif //MATRIX_H
