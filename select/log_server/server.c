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
#include <sys/shm.h>
#include <sys/select.h>
#include "mysem.c"
#define timeout 500
#define no_of_services 5
#define max_client 500
//server code for multiserver service system
int f[max_client];
int fid[max_client];
int ffid[max_client];
int mxfd;

int max(int a, int b){
	return (a>b?a:b);
}
char* services[no_of_services];
char* services_exe[no_of_services];
int main(int argc,char* argv[])
{
	//**description of servies provided by server
	int i;
	for(i=0;i<no_of_services;i++)
	{
		services[i]=(char*)malloc(sizeof(char)*100);
		services_exe[i]=(char*)malloc(sizeof(char)*100);
	}
	services[0]="check whether a number is prime or not";
	services[1]="find nth fibonacci number";
	services[2]="value of pi";
	services_exe[0]="./s2";
	services_exe[1]="./s1";
	services_exe[2]="./s3";

	char* shared;
	key_t key = ftok(".",4321);
	int shmid=shmget(key,1024,IPC_CREAT|0666);
	shared=shmat(shmid,NULL,0);

	char servicelist[100];
	int j,k=0;
	for(i=0;i<3;i++)
	{
		for(j=0;j<strlen(services[i]);j++)
		{
			servicelist[k++]=services[i][j];
		}
		if(i!=2)
		servicelist[k++]=':';
	}
	servicelist[k]='\0';
	
	strncpy(shared,servicelist,sizeof(servicelist));
	//** desc ends

	//server client fifo
	printf("server started:\n");
	mkfifo("famousfifo.txt",0666);
	int serverfd=open("famousfifo.txt",0666);
	
	char* pth;
	pth=(char*)malloc(sizeof(char)*100);
	pth=getenv("PWD");
	//poll fifo file
	// struct pollfd fds[1];
	// fds[0].events=POLLIN;
	// fds[0].fd=serverfd;
	fid[0]=serverfd;
	mxfd=max(mxfd, serverfd);
	fd_set rset;
	int n=1;

	while(1)
	{
		FD_ZERO(&rset);
		for(i=0; i<n; i++){
			FD_SET(fid[i], &rset);
		}
		int ret;
		ret=select(mxfd+1, &rset, NULL, NULL, NULL);

		if(ret>0)
		{
			char buf[100];
			read(serverfd,buf,sizeof(buf));
			int l=strlen(buf);
			char servicenum[100];
			char input[100];
			int flag=0,p=0,j,k;
			char id[30];
			int q;
			for(j=0;j<l;j++)
			{
				if(buf[j]==' ')
				{
					q=j;
					break;
				}
				else
				id[j]=buf[j];
			}
			id[j]='\0';
			int pd=atoi(id);
			printf("client id %d is requesting for some service...\n",pd);
			int y=0;
			for(j=q+1;j<l;j++)
			{
				if(buf[j]==':')
				{
					flag=1;
				}
				else
				{

					if(flag==0)
						servicenum[y++]=buf[j];
					else
					{
						input[p++]=buf[j];
					}
				}
			}
			servicenum[y]='\0';
			input[p]='\0';
			int servicenumber=atoi(servicenum);
			printf("client id %d requested for service number %d:\n",pd,servicenumber+1);
			char rfilename[40],wfilename[40];
			sprintf(rfilename,"%s%s",id,"rdfamousfifo.txt");
			sprintf(wfilename,"%s%s",id,"wrfamousfifo.txt");
			int wfd=open(rfilename,0666);
			int rfd=open(wfilename,0666);
			dup2(rfd,0);
			FILE* f;
			printf("executing the requested service....\n");
			f=popen(services_exe[servicenumber],"r");
			printf("requested service executed successfully....\n");
			char ans[40];
			fgets(ans,40,f);
			printf("sending the output of the service to the client....\n");
			printf("answer:");
			printf("%s\n",ans);
			write(wfd,ans,sizeof(ans));
			printf("output sent successfully....\n");
		}

	}
	return 0;
}