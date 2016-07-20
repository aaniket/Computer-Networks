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

int main(){
	mkfifo("ff.txt",0666);
	int ffd=open("ff.txt",0666);
	struct pollfd fds[max_client];
	fds[0].fd=ffd;
	fds[0].events=POLLIN;

	nfds_t n=1;

	int clid[max_client];
	int rfd[max_client];
	int wfd[max_client];

	while(1){
		int ret=poll(fds,n,timeout);
		if(ret>0){
			if(fds[0].revents&POLLIN){

				char buffer[40];
				read(fds[0].fd,buffer,sizeof(buffer));
				char buffer1[50];
				char buffer2[50];
				sprintf(buffer1,"ff%s.txt",buffer);
				sprintf(buffer2,"ff1%s.txt",buffer);
				mkfifo(buffer1,0666);
				int rf=open(buffer1,0666);
				mkfifo(buffer2,0666);
				int wf=open(buffer2,0666);

				fds[n].fd=rf;
				fds[n].events=POLLIN;
				rfd[n-1]=rf;
				wfd[n-1]=wf;
				clid[n-1]=atoi(buffer);
				n++;

			}
			else{
				int i=0;
				int val=0;
				for(i=0;i<n;i++){
					if(fds[i].revents&POLLIN){
						val=i;
						break;
					}
				}	
				char buffer[40];
				char buffer1[50];
				read(rfd[val],buffer,sizeof(buffer));
				sprintf(buffer1,"client %d:%s",clid[val],buffer);


				for(i=0;i<n;i++){
					if(i!=val){
						write(wfd[i],buffer1,sizeof(buffer1))
					}
				}

			}
		}
	}
	return 0;
}