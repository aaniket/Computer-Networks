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
	int platformid[3];
};
struct sharedmem* shared;
struct service
{
	int sid;
	int is_alloted;
	int size;
};

struct service *s[5];
char* rfilename[5];
char* wfilename[5];
int fl=0;
void handler()
{
	fl=1;
	signal(SIGUSR2,handler);
}

int main()
{
	rfilename[0]="rdfamousfifo1.txt";
	wfilename[0]="wrfamousfifo1.txt";
	rfilename[1]="rdfamousfifo2.txt";
	wfilename[1]="wrfamousfifo2.txt";
	rfilename[2]="rdfamousfifo3.txt";
	wfilename[2]="wrfamousfifo3.txt";
	signal(SIGUSR2,handler);
	key_t key = ftok(".",3429);
	int shmid=shmget(key,1024,IPC_CREAT|0666);
	shared=(struct sharedmem*)shmat(shmid,NULL,0);
	int serverid=shared->serverid;
	//printf("%d\n",shared->serverid);
	
	int num;
	printf("Enter the number of platforms on station:");
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
	int pid,id;
	for(i=0;i<num;i++)
	{
		pid=0;
		pid=fork();
		if(pid>0)
		{
			shared->platformid[i]=pid;
			//printf("%d\n",shared->platformid[i]);
		}
		//printf("%d\n",pid);
		if(pid==0)
		{
			unlink(rfilename[i]);
			unlink(wfilename[i]);
			int id=getpid();
			//printf("%d\n",id);
			//printf("%s %s\n",rfilename[i],wfilename[i]);
			mkfifo(rfilename[i],0666);
			mkfifo(wfilename[i],0666);
			//printf("%d\n",rfd);
			int key=i;
			//printf("%d\n",i);
			while(1)
			{
				while(fl==0){}

				int rfd=open(wfilename[i],0666);
				char buf[100];
				//printf("%d\n",rfd);
				//printf("%d\n",i);
				read(rfd,buf,sizeof(buf));
				int len=strlen(buf);
				printf("%s\n",buf);
				int val=buf[len-1]-48;
				//printf("%d\n",val);
				//printf("%d\n",key);
				fl=0;
				if((val-1)==key)
				{
					//printf("%d\n",key);
					s[val-1]->is_alloted=0;
					sleep(3);
					kill(serverid,SIGUSR1);
				}
			}
			break;
		}
	}
	if(pid>0)
	{
		wait();
	}
	return 0;
}