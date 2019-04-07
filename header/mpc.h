#ifndef MPC_H
#define MPC_H

#include "config.h"
#include "solver.h"

void mpc_init(void);
void mpc_update(double x0,double x1,double x2,double x3,double slop,double a21, double b21);

#endif //MPC_H
