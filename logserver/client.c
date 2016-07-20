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
#define no_of_services 5
#include "mysem.c"
char* services[no_of_services];

int main()
{
	int semid=sem_init(2,1);
	char* shared;
	key_t key = ftok(".",4321);
	int shmid=shmget(key,1024,IPC_CREAT|0666);
	shared=shmat(shmid,NULL,0);
	int pid=getpid();
	char id[30];
	sprintf(id,"%d",pid);
	char rfilename[40],wfilename[40];
	sprintf(rfilename,"%s%s",id,"rdfamousfifo.txt");
	sprintf(wfilename,"%s%s",id,"wrfamousfifo.txt");
	mkfifo(rfilename,0666);
	mkfifo(wfilename,0666);
	int rfd=open(rfilename,0666);
	int wfd=open(wfilename,0666);
	int i;
	for(i=0;i<no_of_services;i++)
	{
		services[i]=(char*)malloc(sizeof(char)*100);
	}
	sem_change(semid,0,-1);
	int len=strlen(shared);
	int x=0;
	int p=0;
	for(i=0;i<len;i++)
	{
		if(shared[i]==':')
		{
			p=0;
			x++;
		}
		else
		services[x][p++]=shared[i];
	}
	sem_change(semid,0,1);
	int clientfd=open("famousfifo.txt",0666);
	int n;
	int fl=1;
	while(fl==1)
	{
		printf("Choose one service you want to use from given servies:\n");
		for(i=0;i<=x;i++)
		{
			printf("%d %s %s\n",i+1,":",services[i]);
		}
		printf("enter your choice:");
		scanf("%d",&n);
		char choicenum[10],choice[20];
		sprintf(choicenum,"%d",n-1);
		sprintf(choice,"%s%s%s",id," ",choicenum);

		sem_change(semid,1,-1);
		write(clientfd,choice,sizeof(choice));
		sem_change(semid,1,1);
		if(n==3)
		{
			char ans[100];
			read(rfd,ans,sizeof(ans));
			printf("%s\n",ans);
		}
		else
		{
			int m;
			char ans[100];
			printf("enter the number:");
			scanf("%d",&m);
			char input[10];
			sprintf(choicenum,"%d",n-1);
			sprintf(input,"%d",m);
			write(wfd,input,sizeof(input));
			read(rfd,ans,sizeof(ans));
			printf("%s\n",ans);
		}
		printf("do u want to contunue to use services(1/0):");
		scanf("%d",&fl);
	}
	unlink(rfilename);
	unlink(wfilename);
	return 0;
}