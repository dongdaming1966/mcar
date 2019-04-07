#ifndef KALMAN_H
#define KALMAN_H

double * kalman(double output,double input);
double * kalman_sys(double output[],double input);
double * kalman_est(double output,double bvel,double macc);

#endif //KALMAN_H
