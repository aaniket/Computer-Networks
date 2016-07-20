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
#include <signal.h>
#include "mysem.c"
struct sharedmem
{
	int serverid;
	int cid;
};

struct sharedmem *shared;
int clientid;
int rfd;
int wfd;
int semid;
int fl=0;
void handler()
{
	clientid=shared->cid;
	sem_change(semid,1,1);
	fl=1;
	char id[30];
	sprintf(id,"%d",clientid);
	char rfilename[40],wfilename[40];
	sprintf(rfilename,"%s%s",id,"rdfamousfifo.txt");
	sprintf(wfilename,"%s%s",id,"wrfamousfifo.txt");
	rfd=open(rfilename,0666);
	wfd=open(wfilename,0666);
}

int main()
{
	key_t key = ftok(".",3429);
	int shmid=shmget(key,1024,IPC_CREAT|0666);
	shared=(struct sharedmem*)shmat(shmid,NULL,0);
	int serverid=shared->serverid;
	semid=sem_init(3,1);
	int servfd=open("servicefifo.txt",0666);
	
	int num;
	printf("Enter the number of service instances you want to provide to client:");
	scanf("%d",&num);
	int i;
	for(i=0;i<num;i++)
	{
		int pid=fork();
		char id[30];
		sprintf(id,"%d",pid);
		sem_change(semid,0,-1);
		write(servfd,id,sizeof(id));
		sem_change(semid,0,1);
		if(pid==0)
		{
			signal(SIGUSR1,handler);
			while(fl!=1)
			{
				char buf[10];
				read(rfd,buf,sizeof(buf));
				int n=atoi(buf);
				if(n%2==0)
					write(wfd,"even number",sizeof("even number"));
				else
					write(wfd,"odd number",sizeof("odd number"));

				kill(serverid,SIGUSR2);
				fl=1;
			}
		}
	}
	return 0;
}