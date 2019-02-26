#ifndef FILE_H
#define FILE_H

#define		MAXLOADBUFF 9999

int file_loadfir(char *path,double *buff);
void file_loadpara(char *path,int num,double *buff);
void file_savepara(char *path,int num,double *buff);

#endif //FILE_H
