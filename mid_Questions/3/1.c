#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#define timeout 500
int fdarr[4];
int main()
{
	struct pollfd fds[5];
	FILE* f1=popen("./p2","r");
	FILE* f2=popen("./p3","r");
	FILE* f3=popen("./p4","r");
	FILE* f4=popen("./p5","r");
	fdarr[0]=fileno(f1);
	fdarr[1]=fileno(f2);
	fdarr[2]=fileno(f3);
	fdarr[3]=fileno(f4);
	int i;
	for(i=0;i<4;i++)
	{
		fds[i].fd=fdarr[i];
		fds[i].events=POLLIN;
		fds[i].revents=0;
	}	
	fds[4].fd=1;
	fds[4].events=POLLIN;
	fds[4].revents=0;
	int fd6[2];
	int pid=0;
	pipe(fd6);
	pid=fork();
	if(pid==0)
	{
		close(fd6[1]);
		dup2(fd6[0],0);
		execlp("./p6","./p6",NULL);
	}
	close(fd6[0]);
	int ret=poll(fds,5,0);
	if(ret>0)
	{
		int fl=0;
		for(i=0;i<4;i++)
		{
			if(fds[i].revents & POLLIN)
			{
				if(i!=4)
				{
					char buf[30];
					read(fds[i].fd,buf,sizeof(buf));
					write(fd6[1],buf,sizeof(buf));
					fl=1;
				}
			}
		}
	}
	else
	{
		int x=dup(1);
		dup2(fd6[1],1);
		write(1,"mrityunjay",sizeof("mrityunjay"));
		dup2(x,1);
	}
	return 0;
}