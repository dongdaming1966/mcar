//File name:	sys.c
//Author:	Dong Daming

#include "sys.h"

//******************************************
//Name:		sys_welcome
//Parameter:	void
//Return:	void
//Description:	print mcat logo. 
//******************************************
void sys_welcome()
{
	printf("\n ---.---.---   --------    ------   ---  ---\n");
	printf("|   _   __  \\ /   _____| /   __   \\|   |/  /\n");
	printf("|  | | |  |  |   /      |   /  \\   |      /\n");
	printf("|  | | |  |  |  |       |  |    |  |    _/\n");
	printf("|  | | |  |  |   \\_____ |   \\__/   |   | \n");
	printf("|__| |_|  |__|\\________| \\______/\\_|___|\n\n");
}

//******************************************
//Name:		sys_para
//Parameter:	void
//Return:	void
//Description:	change or print parameters
//******************************************
void sys_para()
{
	char input[COMMAXLEN];
	char com[COMMAXNUM][COMMAXLEN]={"h",		//index 0
					"c",    	//index	1
					"p",		//index 2
					"q",		//index 3
					"l",		//index 4
					"s"		//index 5
					};

	double num;
	int index;
	int para_run=1;

	while(para_run)
	{
		printf("\npara >> ");

		scanf("%s",input);
		for(index=0;index<COMMAXNUM;index++)
		{
			if(!strcmp(input,com[index]))
			break;
		}
		switch(index)
		{
			case 0:
				printf("  command |  name  |  description\n");
				printf("---------------------------------\n");
				printf("     c    | change | change parameters\n");
				printf("     h    |  help  | print this map.\n");
				printf("     l    |  load  | load parameters from file.\n");
				printf("     p    |  print | print parameters are used now\n");
				printf("     q    |  quit  | back to main program\n");
				printf("     s    |  save  | save parameters into file.\n");
				break;

			case 1:
				scanf("%d%lf",&index,&num);
				if(index<para_num)
					para[index]=num;
				else
					printf("[SYS] error: index %d %lf is out of range!\n",index,num);
				break;

			case 2:
				printf("\n*******************************************\n\n");
#ifdef PID
				printf("PID CONTROLLER\n");
				printf("[0] proportion:%lf\n[1] differetion:%lf\n[2] velocity gain:%lf\n[3] position gain: %lf\n",para[0],para[1],para[2],para[3]);

				printf("\n*******************************************\n\n");
#endif
				printf("SWIP SIGNAL\n");
				printf("[4] amplitude:%lf\n[5] frequency:%lf\n",para[4],para[5]);
				printf("\n*******************************************\n\n");
				printf("IMU\n");
				printf("[6] angle bias:%lf\n",para[6]);
				printf("\n*******************************************\n\n");
#ifdef LINEARPID
				printf("PID CONTROLLER with FEEDBACK LINEARZATION\n");
				printf("[7] proportion:%lf\n[8] differetion:%lf\n[9] velocity gain:%lf\n[10] position gain: %lf\n",para[7],para[8],para[9],para[10]);
				printf("\n*******************************************\n\n");
#endif
#ifdef SLIDINGMODE 
				printf("SLIDING MODE CONTROLLER\n");
				printf("[11] system parameter:%lf\n[12] proportion:%lf\n",para[11],para[12]);
				printf("\n*******************************************\n\n");
#endif
#ifdef SLIDINGMODEPID
				printf("SLIDING MODE with PID CONTROLLER\n");
				printf("[13] proportion:%lf\n[14] differetion:%lf\n[15] tolarent:%lf\n[16] amplitude:%lf\n",para[13],para[14],para[15],para[16]);
				printf("\n*******************************************\n\n");
#endif
#ifdef LINEARLQR
				printf("LQR CONTROLLER with FEEDBACK LINEARZATION\n");
				printf("[17] linear part:%lf\n[18] gravity part:%lf\n[19] body acc compensation:%lf\n[20] gain:%lf\n",para[17],para[18],para[19],para[20]);
				printf("\n*******************************************\n\n");
#endif
#ifdef MPC
				printf("MPC CONTROLLER\n");
				printf("[21] output amplify:%lf\n",para[21]);
				printf("[22] slop:%lf\n",para[22]);
				printf("[23] wheel_pos:%lf\n",para[23]);
				printf("[24] max_wheel_pos:%lf\n",para[24]);
				printf("\n*******************************************\n\n");
#endif
				break;

			case 3:
				para_run=0;
				break;

			case 4:
				file_loadpara("para.cfg",para_num,para);
				break;

			case 5:
				file_savepara("para.cfg",para_num,para);
				break;

	
			default:
				printf("command are not recongized. you can type \"h\" to get help information.\n");
		}
	}
}

//******************************************
//Name:		sys_interface
//Parameter:	void
//Return:	void	
//Description:	collect user's input information, 
//		then start the function needed.
//******************************************
void* sys_interface(void* arg)
{
	char com[COMMAXNUM][COMMAXLEN]={"h",		//index 0
					"q",		//index 1
					"p",		//index	2
					"b",		//index 3
					"s",		//index 4
					"c"		//index 5
					};
	char input[COMMAXLEN];
	int index;
	
	void *ret;

	pthread_t bal,swp,cali;

	printf("you can press \"h\" for help information.\n");
	while(sys_run)
	{
		printf(">> ");
		scanf("%s",input);
		for(index=0;index<COMMAXNUM;index++)
		{
			if(!strcmp(input,com[index]))
				break;
		}

		switch(index)
		{
			case 0:
				printf("  command |  name   |  description\n");
				printf("---------------------------------\n");
				printf("     b    | balance | toggle balance proccess status\n");
				printf("     c    |calibrate| calibrate imu.\n");
				printf("     h    |  help   | print this help menu, can also be used in other modes to print others help menu\n");
				printf("     p    |  para   | enter  parameters adjustment mode\n");
				printf("     q    |  quit   | quit this program\n");
				printf("     s    |  sweep  | start target angle sweeping program\n");
				break;

			case 1: 
				if(balance_run)
				{
					balance_run=0;
					pthread_join(bal,&ret);
				}

				if(cali_run)
				{
					cali_run=0;
					pthread_join(cali,&ret);
				}
				sys_run=0;
				break;

			case 2:
				sys_para();
				break;

			case 3:
				if(balance_run==0)
				{
					balance_run=1;
					printf("[SYS] Balance proccess started.\n");
					pthread_create(&bal,NULL,balance,NULL);	//start balance process	

				}
				else
				{
					balance_run=0;
					printf("[SYS] Balance proccess stopped.\n");
				}
				break;

			case 4 :
				pthread_create(&swp,NULL,sweep,NULL);	//start sweep process	
				break;

			case 5 :
				if(cali_run==0)
				{
					cali_run=1;
					printf("[SYS] Calibrate IMU proccess started.\n");
					pthread_create(&cali,NULL,calibrate_imu,NULL);	//start balance process	

				}
				else
				{
					cali_run=0;
					printf("[SYS] Calibrate IMU proccess stopped.\n");
				}
				break;

			default: 
				printf("command are not recongized. you can type \"h\" to get help information.\n");
		}
	}

	return 0;
}
