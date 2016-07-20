#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#define max_client 5
#define timeout 500
//server code for chat networking system
int main(){
	printf("server started:\n");
	
	mkfifo("famousfifo.txt",0666);
	int ffd=open("famousfifo.txt",0666);
	char *s[5]={"stdin","fifo","pipe","popen",""};
	struct pollfd fds[max_client];//stdin,fifo,pipe,popen
	fds[0].events=POLLIN;
	fds[0].fd=0;
	fds[1].events=POLLIN;
	fds[1].fd=ffd;
	int pfd[2];
	pipe(pfd);
	int cid=fork();
	if(cid==0){
		close(pfd[0]);
		while(1){
			sleep(3);
			char buf[10];
			sprintf(buf, "hello");
			//printf("here\n");
			//printf("PIPE I/P\n");
			write(pfd[1],buf,sizeof(buf));
		//	printf("here\n");
		}
	}
	else{
		close(pfd[1]);
		fds[2].fd=pfd[0];
		fds[2].events=POLLIN;
		nfds_t n=3;
		int i;
		while(1){
			int ret=poll(fds,n,timeout);
			if(ret>0){
				//printf("poled\n");
				for(i=0;i<n;i++){
					if(fds[i].revents & POLLIN){
						char rbuf[512];
						read(fds[i].fd,rbuf,sizeof(rbuf));
						printf("input recieved through %s\n",s[i] );
						printf("input is %s\n", rbuf);
					}
				}
			}
		}
	}
	return 0;
}