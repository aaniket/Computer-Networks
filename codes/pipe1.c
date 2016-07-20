#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <poll.h>
int main(){
	int fd[2];
	pipe(fd);
	int pid=fork();
	assert(pid>=0);
	struct pollfd fds[2];
	fds[0].fd=fd[0];
	int timeout=500;
	fds[0].events=POLLIN;
	if(pid==0){
		close(fd[0]);
		char *pth=getenv("PWD");
		char path[100];
		sprintf(path,"%s/%s",pth,"pipe2");
		dup2(fd[1],1);
		char dummy[100];
		sprintf(dummy,"heybuddy");
		printf("starting pipe2...\n");
		execl(path,dummy,NULL);
	}
	else{
		close(fd[1]);
		while(1){
			printf("waiting...\n");
				int ret=poll(fds,1,timeout);
				if(ret>0){
					char buffer[1024];
					int sz=read(fds[0].fd,buffer,sizeof(buffer));
					printf("%s",buffer);
				}
		}
	}
	return 0;
}