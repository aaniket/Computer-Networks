#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#define max_client 1000
#define timeout 500

int f[max_client];
int fid[max_client];
int ffid[max_client];
int mxfd;

int max(int a, int b){
	return (a>b?a:b);
}

//server code for chat networking system
int main()
{
	printf("server started:\n");
	mkfifo("famousfifo.txt",0666);
	int ffd=open("famousfifo.txt",0666);
	
	fid[0]=ffd;
	mxfd=max(mxfd, ffd);
	fd_set rset;
	int n=1;

	int i;
	while(1)
	{
		FD_ZERO(&rset);
		for(i=0; i<n; i++){
			FD_SET(fid[i], &rset);
		}
		int ret;
		ret=select(mxfd+1, &rset, NULL, NULL, NULL);

		printf("%d\n",ret);
		if(ret>0)
		{
			if(FD_ISSET(fid[0], &rset))
			{
				char id[30];
				read(fid[0],id,sizeof(id));
				char filename[40];
				char filename1[40];
				printf("client id %s requesting the service\n",id);
				sprintf(filename,"%s%s",id,"famousfifo.txt");
				sprintf(filename1,"%s%s",id,"famousfifo1.txt");
				unlink(filename);
				unlink(filename1);
				mkfifo(filename,0666);
				mkfifo(filename1,0666);
				int fdc=open(filename,0666);
				int fdc1=open(filename1,0666);
				
				int idc=atoi(id);
				ffid[n]=idc;
				
				fid[n]=fdc;
				mxfd=max(mxfd, fdc);
				f[n]=fdc1;
				n++;
			}
			else
			{
				char buf[30];
				int val,j;
				for(i=1;i<n;i++)
				{
					if(FD_ISSET(fid[i], &rset))
					{
						val=fid[i];
						j=i;
						read(val,buf,sizeof(buf));
						break;
					}
				}
				char msg[60];
				char clientid[10];
				sprintf(clientid,"%d",ffid[j]);
				sprintf(msg,"%s%s%s%s","client id ",clientid,": ",buf);
				printf("client id %d sending msg to chat group..\n",ffid[j]);
				for(i=1;i<n;i++)
				{
					if(i!=j)
					{
						write(f[i],msg,sizeof(msg));
					}
				}
				printf("client id %d sent msg to chat group..\n",ffid[j]);
			}
		}
	}
	return 0;
}