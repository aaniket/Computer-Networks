#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/signal.h>
#include <sys/shm.h>
#include "mysem.c"
#define timeout 500
#define no_of_services 10

int f[max_client];
int fid[max_client];
int ffid[max_client];
int mxfd;

int max(int a, int b){
	return (a>b?a:b);
}


//server code for service co-ordinator system
struct service
{
	int sid;
	int is_alloted;
	int size;
};

struct sharedmem
{
	int serverid;
	int cid;
};
int cnt=0;
struct service *s[5];
int is_available()
{
	int i,fl=0;
	for(i=0;i<cnt;i++)
	{
		if(s[i]->is_alloted==0)
		{
			s[i]->is_alloted=1;
			return s[i]->sid;
		}
	}
	return -1;
}


int main(int argc,char* argv[])
{

	int semid=sem_init(3,1);
	struct sharedmem* shared;
	key_t key = ftok(".",3429);
	int shmid=shmget(key,1024,IPC_CREAT|0666);
	shared=(struct sharedmem*)shmat(shmid,NULL,0);
	shared->serverid=(int)getpid();
	int m=5190,i;
	for(i=0;i<no_of_services;i++)
	{
		key_t key1 = ftok(".",m);
		int shmid1=shmget(key1,1024,IPC_CREAT|0666);
		s[i]=(struct service*)shmat(shmid1,NULL,0);
		m=m+10;
	}

	printf("server started:\n");
	mkfifo("servicefifo.txt",0666);
	int servfd=open("servicefifo.txt",0666);
	mkfifo("famousfifo.txt",0666);
	int fd=open("famousfifo.txt",0666);
	mkfifo("famousfifo1.txt",0666);
	int fd1=open("famousfifo1.txt",0666);
	//poll fifo files
	fid[0]=servfd;
	mxfd=max(mxfd, servfd);
	fid[1]=fd;
	mxfd=max(mxfd, fd);
	fid[2]=fd1;
	mxfd=max(mxfd, fd1);
	int n=1;
	fd_set rset;
	while(1)
	{
		FD_ZERO(&rset);
		for(i=0; i<n; i++){
			FD_SET(fid[i], &rset);
		}
		int ret;
		ret=select(mxfd+1, &rset, NULL, NULL, NULL);

		//printf("%d\n",ret);
		if(ret>0)
		{
			if(FD_ISSET(fid[0], &rset))
			{
		//		printf("services\n");
				char sid[30];
				read(servfd,sid,sizeof(sid));
				int id=atoi(sid);
		//		printf("%d\n",id);
				s[cnt]->sid=id;
				s[cnt]->is_alloted=0;
				s[0]->size=cnt+1;
				cnt++;
			}
			else if(FD_ISSET(fid[1], &rset))
			{
		//		printf("client\n");
				int serviceid=is_available();
		//		printf("service id:%d",serviceid);
				if(serviceid!=-1)
				{
					write(fd1,"1",sizeof("1"));
		//			printf("available\n");
					char pd[30];
					read(fd,pd,sizeof(pd));
					int id=atoi(pd);
		//			printf("%d\n",id);
					sem_change(semid,1,-1);
					shared->cid=id;
					char rfilename[40],wfilename[40];
					sprintf(rfilename,"%s%s",pd,"rdfamousfifo.txt");
					sprintf(wfilename,"%s%s",pd,"wrfamousfifo.txt");
					mkfifo(rfilename,0666);
					mkfifo(wfilename,0666);
					int wfd=open(rfilename,0666);
					int rfd=open(wfilename,0666);
		//			printf("sending signal\n");

					kill(serviceid,SIGUSR1);
				}
				else
				{
					write(fd1,"0",sizeof("0"));
				}
			}
		}
	}
	return 0;
}