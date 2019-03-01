#ifndef MPC_H
#define MPC_H

#include "config.h"
#include "solver.h"

void mpc_init(void);
void mpc_update(double x0,double x1,double slop);

#endif //MPC_H
