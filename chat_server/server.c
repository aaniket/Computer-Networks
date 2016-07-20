#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#define max_client 1000
#define timeout 500

//server code for chat networking system
int main()
{
	printf("server started:\n");
	mkfifo("famousfifo.txt",0666);
	int ffd=open("famousfifo.txt",0666);
	struct pollfd fds[max_client];
	fds[0].events=POLLIN;
	fds[0].fd=ffd;
	int f[max_client];
	int fid[max_client];
	int i;
	nfds_t n;
	n=1;
	while(1)
	{
		int ret;
		ret=poll(fds,n,timeout);
		printf("%d\n",ret);
		if(ret>0)
		{
			if(fds[0].revents & POLLIN)
			{
				char id[30];
				read(fds[0].fd,id,sizeof(id));
				char filename[40];
				char filename1[40];
				printf("client id %s requesting the service\n",id);
				sprintf(filename,"%s%s",id,"famousfifo.txt");
				sprintf(filename1,"%s%s",id,"famousfifo1.txt");
				mkfifo(filename,0666);
				mkfifo(filename1,0666);
				int fdc=open(filename,0666);
				int fdc1=open(filename1,0666);
				fds[n].fd=fdc;
				fds[n].events=POLLIN;
				int idc=atoi(id);
				fid[n]=idc;
				f[n]=fdc1;
				n++;
			}
			else
			{
				char buf[30];
				int val,j;
				for(i=1;i<n;i++)
				{
					if(fds[i].revents & POLLIN )
					{
						val=fds[i].fd;
						j=i;
						read(val,buf,sizeof(buf));
						break;
					}
				}
				char msg[60];
				char clientid[10];
				sprintf(clientid,"%d",fid[j]);
				sprintf(msg,"%s%s%s%s","client id ",clientid,": ",buf);
				printf("client id %d sending msg to chat group..\n",fid[j]);
				for(i=1;i<n;i++)
				{
					if(fds[i].fd!=val)
					{
						write(f[i],msg,sizeof(msg));
					}
				}
				printf("client id %d sent msg to chat group..\n",fid[j]);
			}
		}
	}
	return 0;
}