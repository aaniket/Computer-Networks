#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>
#include <sys/select.h>


#define BUFFSIZE 512
#define maxs 105

int sfd[maxs], nsfd, mxfd;
char path[BUFFSIZE][BUFFSIZE];

struct sockaddr_in server_addr, client_addr;
fd_set rset;

int main(){
	int n;
	scanf("%d", &n);
	
	int i, port, type, mxcl;
	
	for(i=0; i<n; i++){
		//printf("%d\n", i);
		scanf("%d %d %d %s", &port, &type, &mxcl, path[i]);

		sfd[i]=socket(AF_INET, SOCK_STREAM, 0);
		assert(sfd[i]>=0);

		server_addr.sin_port=htons(port);
		server_addr.sin_family=AF_INET;
		server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
		int status=bind(sfd[i], (struct sockaddr*)&server_addr, sizeof server_addr);

		assert(status>=0);
		listen(sfd[i], mxcl);
	}

	printf("--Sockets created--\n");

	while(1){
		FD_ZERO(&rset);
		mxfd=0;
		for(i=0; i<n; i++){
			FD_SET(sfd[i], &rset);
			if(sfd[i]>mxfd)mxfd=sfd[i];
		}
		int res=select(mxfd+1, &rset, NULL, NULL, 0);
		if(res>0){
			for(i=0; i<n; i++){
				if(FD_ISSET(sfd[i], &rset)){
					int client_len;
					nsfd=accept(sfd[i], (struct sockaddr*)&client_addr, &client_len);

					printf("--Request accepted--\n");
					int p=fork();
					assert(p>=0);
					
					if(p==0){//child
						int j;
						for(j=0; j<n; j++){
							close(sfd[j]);
						}
						char cmnd[BUFFSIZE];
						sprintf(cmnd, "%s%s", "./", path[i]);

						//printf("%s\n", cmnd);
						dup2(nsfd, 0);
						FILE* fp=popen(cmnd, "r");
						int fd=fileno(fp);

						char ans[BUFFSIZE];
						read(fd, ans, sizeof ans);

						printf("The answer is %s\n", ans);
						send(nsfd, ans, sizeof ans, 0);
					}
					else{
						close(nsfd);
					}
					break;
				}
			}
		}
	}

	return 0;
}