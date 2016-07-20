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
#include <sys/signal.h>
struct sharedmem
{
	int serverid;
	int cid;
};
struct sharedmem* shared;
int clientid;
int rfd;
int wfd;
int semid;
int fl=0;
int spid;
void handler()
{
	//printf("handler\n");
	spid=getpid();
	clientid=shared->cid;
	//printf("%d\n",clientid);
	sem_change(semid,1,1);
	fl=1;
	char id[30];
	sprintf(id,"%d",clientid);
	char rfilename[40],wfilename[40];
	sprintf(rfilename,"%s%s",id,"rdfamousfifo.txt");
	sprintf(wfilename,"%s%s",id,"wrfamousfifo.txt");
	rfd=open(rfilename,0666);
	wfd=open(wfilename,0666);
	signal(SIGUSR1,handler);
}
struct service
{
	int sid;
	int is_alloted;
	int size;
};
struct service *s[5];

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
	int m=5190;
	for(i=0;i<5;i++)
	{
		key_t key1 = ftok(".",m);
		int shmid1=shmget(key1,1024,IPC_CREAT|0666);
		s[i]=(struct service*)shmat(shmid1,NULL,0);
		m=m+10;
	}
	int cnt=0;
	int j;
	for(i=0;i<num;i++)
	{
		int pid=0;
		pid=fork();
		if(pid>0)
		{
			char id[30];
			sprintf(id,"%d",pid);
			sem_change(semid,0,-1);
			write(servfd,id,sizeof(id));
			sem_change(semid,0,1);
		}
		else if(pid==0)
		{
			while(1)
			{
				//printf("child\n");
				signal(SIGUSR1,handler);
				while(fl==0)
				{

				}
				//printf("come here\n");
				char buf[10];
				read(rfd,buf,sizeof(buf));
				int n=atoi(buf);
				sleep(2);
				//printf("%d\n",n);
				if(n%2==0)
					write(wfd,"the entered number is even.",sizeof("the entered number is even."));
				else
				write(wfd,"the entered number is odd.",sizeof("the entered number is odd."));
				int j;
				for(j=0;j<s[0]->size;j++)
				{
					if(spid==s[j]->sid)
					{
					//	printf("%d\n",spid);
						s[j]->is_alloted=0;
						break;
					}
				}
				fl=0;
			}
			break;
		}
	}
	return 0;
}