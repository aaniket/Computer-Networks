#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/signal.h>
// code contains process 3 and 4
int main()
{
	unlink("rdfifo.txt");
	mkfifo("rdfifo.txt",0666);
	int fd=open("wrfifo.txt",0666);
	int fd1=open("rdfifo.txt",0666);
	char id[30];
	int serverid;
	read(fd,id,sizeof(id));
	printf("%s\n",id);
	serverid=atoi(id);
	printf("%d\n",serverid);
	int pid=0;
	pid=fork();
	if(pid==0)
	{
		int n;
		while(1)
		{
			printf("enter any integer for sending the signal:");
			scanf("%d",&n);
			kill(serverid,SIGUSR1);
		}
		wait();
	}
	else
	{
		while(1)
		{
			write(fd1,"fifodata\n",sizeof("fifodata\n"));
			//sleep(1);
		}
	}
	return 0;
}