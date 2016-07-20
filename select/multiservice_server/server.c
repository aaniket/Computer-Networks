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
int f[max_client];
int fid[max_client];
int ffid[max_client];
int mxfd;

int max(int a, int b){
	return (a>b?a:b);
}

int main(){
	mkfifo("famousfifo.txt",0666);
	int ffd=open("famousfifo.txt",0666);
	fid[0]=ffd;
	mxfd=max(mxfd, ffd);
	fd_set rset;
	int n=1;
	int i;
	//int n=1;
	//int f[max_client],fid[max_client];
	while(1){
		
		
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
				char val[300];
				char id[30];
				char service[30];
				read(fid[0],val,sizeof(val));
				int len=strlen(val);
				printf("value %s %d\n", val,len);
				int i;
				for(i=0;i<len;i++){
					if(val[i]=='|'){
						break;
					}
					else{
						id[i]=val[i];
					}
				}
				id[i]='\0';
				int j=0;
				i++;
				for(i;i<len;i++){
					service[j++]=val[i];
				}
				service[j]='\0';
				char filename[40];
				char filename1[40];
				printf("client id %s requesting to join..\n",id);
				sprintf(filename,"%s%s",id,"famousfifo.txt");
				sprintf(filename1,"%s%s",id,"famousfifo1.txt");
				mkfifo(filename,0666);
				mkfifo(filename1,0666);
				int fdc=open(filename,0666);
				int fdc1=open(filename1,0666);
				fid[n]=fdc;
				
				int idc=atoi(id);
				ffid[n]=idc;
				mxfd=max(mxfd, fdc);
				f[n]=fdc1;
				//n++;
				n++;
				printf("%s\n",service );
				pid_t pid=fork();
				if(pid==0){
					char* pth=getenv("PWD");
					char path[100];
					sprintf(path,"%s/%s",pth,service);
					printf("%s\n",path);
					printf("forked\n");
					dup2(fdc,0);
					dup2(fdc1,1);
					char* dummy;
					execl(path,dummy,NULL);
					printf("here\n");
				}
			}
		}

	}
 	return 0;
 }