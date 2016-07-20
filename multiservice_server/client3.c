#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

void readfn(char* pd)
{

	char filename[40];
	sprintf(filename,"%s%s",pd,"famousfifo.txt");
	int ffd=open(filename,0666);
	char buf1[30];
	int n=read(0,buf1,sizeof(buf1));
	printf("input is %s\n",buf1 );
	buf1[n]='\0';
	write(ffd,buf1,sizeof(buf1));
}

void writefn(char* pd)
{
	char buf[30];
	char filename1[40];
	sprintf(filename1,"%s%s",pd,"famousfifo1.txt");
	int ffd1=open(filename1,0666);
	read(ffd1,buf,sizeof(buf));
	printf("%s",buf);
	//printf("buffer printed\n");
}

int main()
{
	char pd[30];
	int fd=open("famousfifo.txt",0666);
	pid_t pid;
	pid=getpid();
	char name[]="|service2";
	sprintf(pd,"%d%s",pid,name);
	write(fd,pd,sizeof(pd));
	char id[30];
	sprintf(id,"%d",pid);
	sleep(2);
		readfn(id);
		//sleep(5);
		writefn(id);

	return 0;
}