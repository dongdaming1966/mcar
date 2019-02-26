#ifndef FUNC_H
#define FUNC_H

#define		MOTOR_I_LIMIT 10000000

extern int sys_run;
extern int balance_run;
extern int cali_run;
extern double para[];

int imu_fd,motor_fd;

double abs_double(double data);
void clean(void);
void stop(int signo);
void* balance(void* arg);
void* sweep(void* arg);
void* calibrate_imu(void* arg);

#endif //FUNC_H
