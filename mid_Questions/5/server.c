#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/signal.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#define family AF_INET
#define typ SOCK_STREAM
#define timeout 5000
#define portno 9000

struct pollfd fds[4];
int nsfds[4];
int alloted[4];
int cnt=0;
char buf[100];
int serverid;

void handler1()
{
	printf("handler1\n");
	alloted[cnt++]=1;
	printf("cnt:%d\n",cnt);
	signal(SIGUSR1,handler1);
	int pid2=0;
	pid2=fork();
	if(pid2==0)
	{
		printf("service to clients from echo server..\n");
		write(nsfds[cnt-1],"echo server data\n",sizeof("echo server data\n"));
		write(nsfds[cnt-1],buf,sizeof(buf));
		kill(serverid,SIGUSR2);
	}
}

void handler2()
{
	printf("handler2\n");
	cnt--;
	printf("cnt:%d\n",cnt);
	alloted[cnt]=0;
	signal(SIGUSR2,handler2);
}

int main()
{
	int arr[2];
	pipe(arr);
	int pid=0;
	serverid=getpid();
	pid=fork();
	int status;
	signal(SIGUSR1,handler1);
	signal(SIGUSR2,handler2);
	if(pid>0)
	{
		printf("%d %d\n",serverid,pid);
		int pid1=0;
		int arr1[2];
		pipe(arr1);
		pid1=fork();
		if(pid1==0)
		{
			dup2(arr1[1],1);
			execlp("./p3","./p3",NULL);
		}
		unlink("wrfifo.txt");
		mkfifo("wrfifo.txt",0666);
		int fd=open("wrfifo.txt",0666);
		int pid3=fork();
		if(pid3==0)
		{
			char id[30];
			int fd1;
			printf("%d\n",serverid);
			sprintf(id,"%d",serverid);
			printf("%s\n",id);
			write(fd,id,sizeof(id));
			fd1=open("rdfifo.txt",0666);
			fds[0].fd=fd1;
			fds[0].events=POLLIN;
		}

		fds[1].fd=arr[0];
		fds[1].events=POLLIN;
		fds[2].fd=arr1[0];
		fds[2].events=POLLIN;
		int sfd=socket(family,typ,0);
		fds[3].fd=sfd;
		fds[3].events=POLLIN;
		struct sockaddr_in serveraddr;
		serveraddr.sin_family=family;
		serveraddr.sin_port=htons(portno);
		
		if(inet_aton("127.0.0.1",&serveraddr.sin_addr) ==0)
		{
			printf("error in address binding\n");
		}

		bind(sfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
		listen(sfd,10);
		
		int j=0;
		printf("come here..\n");
		while(1)
		{
			int ret=poll(fds,4,timeout);
			if(ret>0)
			{
				int i;
				for(i=0;i<4;i++)
				{
					if(fds[i].revents & POLLIN)
					{
						if(i==3)
						{
							printf("connection\n");
							struct sockaddr_in peeraddr;
							unsigned int len;
							int nsfd=accept(fds[i].fd,(struct sockaddr*)&peeraddr,&len);
							assert(nsfd>=0);
							nsfds[j++]=nsfd;
						}
						else
						{
							//printf("service to clients from main server..\n");
							int k;
							read(fds[i].fd,buf,sizeof(buf));
							//printf("%s\n",buf);
							for(k=0;k<j;k++)
							{
								if(alloted[k]==0)
								{
									write(nsfds[k],"main server data\n",sizeof("main server data\n"));
									write(nsfds[k],buf,sizeof(buf));
								}
							}
						}
						break;
					}
				}
			}
		}
	}
	else
	{
		while(1)
		{
			write(arr[1],"pipedata\n",sizeof("pipedata\n"));
			sleep(1);
		}
		wait(&status);
	}
	return 0;
}