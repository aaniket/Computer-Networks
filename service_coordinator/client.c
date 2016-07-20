#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "mysem.c"

struct sharedmem
{
	int serverid;
	int cid;
};

struct sharedmem *shared;
int semid;

int main()
{
	key_t key = ftok(".",3429);
	int shmid=shmget(key,1024,IPC_CREAT|0666);
	shared=(struct sharedmem*)shmat(shmid,NULL,0);
	int serverid=shared->serverid;
	semid=sem_init(3,1);
	int fd=open("famousfifo.txt",0666);
	int fd1=open("famousfifo1.txt",0666);
	int pid=getpid();
	char id[30];
	sprintf(id,"%d",pid);
	write(fd,id,sizeof(id));
	char res[3];
	int n=read(fd1,res,sizeof(res));
	res[n]='\0';
	if(res[0]=='0')
	{
		printf("servie not available\n");
		return 0;
	}
	else if(res[0]=='1')
	{
		int n;
		printf("enter the number:");
		scanf("%d",&n);
		char input[10],output[40];
		sprintf(input,"%d",n);
		char rfilename[40],wfilename[40];
		sprintf(rfilename,"%s%s",id,"rdfamousfifo.txt");
		sprintf(wfilename,"%s%s",id,"wrfamousfifo.txt");
		int wfd=open(rfilename,0666);
		int rfd=open(wfilename,0666);
		write(wfd,input,sizeof(input));
		int val=read(rfd,output,sizeof(output));
		write(1,output,val);
		printf("\n");
	}
	return 0;
}