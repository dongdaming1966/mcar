//File name:	sys.c
//Author:	Dong Daming
//Last Edited:	2018/4/17

#include	"common.h"

#ifndef		FILEMOTOR
#include	"motor.c"
#endif

#define		FILESYS

#define		COMMAXNUM 99
#define		COMMAXLEN 20

extern double pid_p,pid_i,pid_d,swp_amp,swp_freq;

//******************************************
//Name:		sys_welcome
//Parameter:	void
//Return:	void
//Description:	print mcat logo. 
//******************************************
void sys_welcome()
{
	printf(" ---.---.---   --------    ------   ---  ---\n");
	printf("|   _   __  \\ /   _____| /   __   \\|   |/  /\n");
	printf("|  | | |  |  |   /      |   /  \\   |      /\n");
	printf("|  | | |  |  |  |       |  |    |  |    _/\n");
	printf("|  | | |  |  |   \\_____ |   \\__/   |   | \n");
	printf("|__| |_|  |__|\\________| \\______/\\_|___|\n");
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
	char com[COMMAXNUM][COMMAXLEN]={"help",		//index 0
					"change"	//index	1
					};
	
	char para[COMMAXNUM][COMMAXLEN]={"pid_p",	//index 0
					"pid_i",	//index 1
					"pid_d",	//index 2
					"swp_amp",	//index 3
					"swp_freq",	//index 4
					};
	double num;

	int index;

	if(getchar()!=' ')
	{
		//didn't enter any parameters then print the parameters
		printf("*******************************************\n");
		printf("CONTROLLER\n");
		printf("pid_p:%lf\tpid_i:%lf\tpid_d:%lf\n",pid_p,pid_i,pid_d);

		printf("*******************************************\n");
		printf("SWIP SIGNAL\n");
		printf("swp_amp:%lf\tswp_freq:%lf\n",swp_amp,swp_freq);
	}
	else
	{
		scanf("%s",&input);
		for(index=0;index<COMMAXNUM;index++)
		{
			if(!strcmp(input,com[index]))
			break;
		}
		switch(index)
		{
			case 0:
				printf("NULL\n");
				break;
			case 1:
				scanf("%s%lf",&input,&num);
				for(index=0;index<COMMAXNUM;index++)
				{
					if(!strcmp(input,para[index]))
					break;
				}

				switch(index)
				{
					case 0:	pid_p=num;break;
					case 1:	pid_i=num;break;
					case 2:	pid_d=num;break;
					case 3:	swp_amp=num;break;
					case 4:	swp_freq=num;break;
					default:
						printf("parameter are not recongnized!\n");

				}

				break;
			default:
				printf("command are not recongnized!\n");
		}
	}
}
//******************************************
//Name:		sys_interface
//Parameter:	void
//Return:	num 	int	whether exit	
//Description:	collect user's input information, 
//		then start the function needed.
//******************************************
int sys_interface()
{
	char com[COMMAXNUM][COMMAXLEN]={"help",		//index 0
					"exit",		//index 1
					"para"		//index	2
					};
	char input[COMMAXLEN];
	int index;

	printf(">>");
	scanf("%s",&input);
	for(index=0;index<COMMAXNUM;index++)
	{
		if(!strcmp(input,com[index]))
			break;
	}

	switch(index)
	{
		case 0:
			printf("index |  name  |  description\n");
			printf("---------------------------------\n");
			printf("  1   |  help  | print this map\n");
			printf("  2   |  exit  | exit the program\n");
			printf("  3   |  para  | change or print parameters\n");
			break;
		case 1: 
			return 0;
		case 2:

			sys_para();
			break;
		default: 
			printf("command are not recongized. you can press help to get help information.\n");
	}

	return 1;

}
