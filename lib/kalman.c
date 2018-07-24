//File name:	kalman.c
//Author:	平衡小车之家(http://shop114407458.taobao.com/)

#include	"common.h"

#define		FILEKALMAN

float K1 =0.02; 
float angle, angle_dot; 	
float Q_angle=0.01;// 过程噪声的协方差
float Q_gyro=0.03;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle=20;// 测量噪声的协方差 既测量偏差
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

//******************************************
//Name:		kalman_imu_init
//Parameter:	acc	float	accelerometer data
//
//Return:	void	
//Description:	Use the angle measured by accelerometer as
//		the starting value.	
//******************************************
void kalman_imu_init(float acc)
{
	angle=acc;
}

//******************************************
//Name:		kalman_imu_filter	
//Parameter:	gyro	float	gyro data
//		acc	float	acclerometer data
//		dt	float	time passed after last calculation
//		data[2]	float	use to return the value calculated
//				data[0]---->angle
//				data[1]---->gyro bias
//Return:	void
//Description:	kalman filter algorithm
//******************************************
void kalman_imu_filter(float gyro,float acc,float dt,float data[2])		
{
	angle+=(gyro - Q_bias) * dt; //先验估计

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
					
	Angle_err = acc - angle;	//zk-先验估计
											
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
														
	E = R_angle + C_0 * PCt_0;
															
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
																
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
																						
	angle	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	angle_dot   = gyro - Q_bias;	 //输出值(后验估计)的微分=角速度

	data[0]=angle;
	data[1]=gyro-Q_bias;
}

