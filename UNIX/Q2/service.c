#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <sys/un.h>

#define maxs 55

struct sockaddr_un addr;
struct sockaddr_in serv, client;

char SOCKET_PATH[512];

int clientfds[maxs];
int clients=0;

pthread_t cth[maxs];

int create_unix_socket(){
	int usfd=socket(AF_UNIX, SOCK_STREAM, 0);

	assert(usfd>=0);

	addr.sun_family=AF_UNIX;
	memcpy(&addr.sun_path, SOCKET_PATH, strlen(SOCKET_PATH));

	unlink(SOCKET_PATH);

	int st=bind(usfd, (struct sockaddr*)&addr, sizeof addr);

	assert(st>=0);

	listen(usfd, 10);

	return usfd;
}

void* service(void* args){
	int* arg=(int*)args;
	int fd=*arg;

	char buff[512];

	while(1){
			int l=read(fd, buff, sizeof buff);
			int i;
			for(i=0; i<l/2; i++){
				char ch=buff[i];
				buff[i]=buff[l-i-1];
				buff[l-i-1]=ch;

			}
			write(fd, buff, l);
	}
}

void* recv_fd(void* args){
	int* arg=(int*)args;
	int unsfd=*arg;

	struct msghdr message;
	struct iovec iov;
	struct cmsghdr *control_msg = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];
	
	memset(&message, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

	iov.iov_base = data;
	iov.iov_len = sizeof(data);

	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_iov = &iov;
	message.msg_iovlen = 1;
	message.msg_control = ctrl_buf;
	message.msg_controllen = CMSG_SPACE(sizeof(int));
	
	control_msg = CMSG_FIRSTHDR(&message);
	control_msg->cmsg_level = SOL_SOCKET;
	control_msg->cmsg_type = SCM_RIGHTS;
	control_msg->cmsg_len = CMSG_LEN(sizeof(int));

	while(1){

 		int x=recvmsg(unsfd, &message, 0);

   		if(x>=0){
   			control_msg = CMSG_FIRSTHDR(&message);
   			int clientfd;
   			memcpy(&clientfd, CMSG_DATA(control_msg), sizeof (int));

   			clientfds[clients]=clientfd;
   			
   			printf("%d fd recieved\n", clientfd);

   			int* ffd=&clientfd;
   			pthread_create(&cth[clients], NULL, service, (void*)ffd);
   			clients++;

   		}
   	}

}

int c=0;
pthread_t fd_recv[maxs];

int main(int argc, char* argv[]){
	strcpy(SOCKET_PATH, argv[1]);

	int usfd=create_unix_socket();

	while(1){
		int cl;
		int unsfd=accept(usfd, (struct sockaddr*)&client, &cl);

		assert(usfd>=0);
		
		int* arg=&unsfd;
   		pthread_create(&fd_recv[c], NULL, recv_fd, (void*)arg);
   		c++;
	}

	return 0;
}