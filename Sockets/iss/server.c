#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "semaphore.c"
#include <assert.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/select.h>

#define mxqueue 10
#define maxs 105
#define BUFFSIZE 105
#define shmkey 555
#define semkey 666

int mxcl[maxs];
char path[BUFFSIZE][BUFFSIZE];
int shmid;
int *cnt;
int mutex[10];
int sfd[maxs];

fd_set rset;
int mxfd;

struct sockaddr_in server_addr, client_addr;

int main(){
	//printf("here\n");
	int n;
	scanf("%d", &n);

	shmid=shmget(shmkey, BUFFSIZE, IPC_CREAT);
	assert(shmid>=0);
	cnt=(int*)shmat(shmid, NULL, 0);

	//printf("done\n");
	int i, port, type;
	

	for(i=1; i<=n; i++)cnt[i]=0;

	for(i=1; i<=n; i++){
		mutex[i]=sem_init(semkey+i, 1, 1);
		assert(mutex[i]>=0);
	}

	
	
	
	for(i=1; i<=n; i++){
		scanf("%d %d %d %s", &port, &type, &mxcl[i], path[i]);

		sfd[i]=socket(AF_INET, SOCK_STREAM, 0);
		assert(sfd[i]>=0);

		server_addr.sin_port=htons(port);
		server_addr.sin_family=AF_INET;
		server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
		int status=bind(sfd[i], (struct sockaddr*)&server_addr, sizeof server_addr);

		assert(status>=0);
		listen(sfd[i], mxqueue);
	}

	printf("Sockets Created\n");

	while(1){
		FD_ZERO(&rset);
		mxfd=0;
		for(i=1; i<=n; i++){
			FD_SET(sfd[i], &rset);
			if(sfd[i]>mxfd)mxfd=sfd[i];
		}
		int res=select(mxfd+1, &rset, NULL, NULL, 0);
		if(res>0){
			for(i=1; i<=n; i++){
				if(FD_ISSET(sfd[i], &rset)){
					sem_wait(mutex[i], 0);
					if(cnt[i]<=mxcl[i]){
						int client_len;
						int nsfd=accept(sfd[i], (struct sockaddr*)&client_addr, &client_len);

						printf("Request Accepted\n");

						int p=fork();
						assert(p>=0);
						if(p==0){//child
							int j;
							for(j=1; j<=n; j++)close(sfd[j]);

							char cmnd[BUFFSIZE];
							sprintf(cmnd, "%s%s", "./", path[i]);

							dup2(nsfd, 0);
							FILE* fp=popen(cmnd, "r");
							int fd=fileno(fp);
							
							cnt[i]++;
							sem_signal(mutex[i], 0);


							char ans[BUFFSIZE];
							read(fd, ans, sizeof ans);
							printf("The ans is %s\n", ans);
							send(nsfd, ans, sizeof ans, 0);
						}
						else{
							close(nsfd);
						}
					}
					else
					sem_signal(mutex[i], 0);
					break;
				}
			}
		}
	}
	return 0;
}