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
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>

#define ID 2
#define shmkey 555
#define maxs 15

int True;

struct SHM{
	int pid[10];
	char file[10][55];
};

struct SHM* shared;
int shmid;

struct sockaddr_un addr;
struct sockaddr_in serv, client;

char SOCKET_PATH[512];

int clientfds[maxs];
int clients=0;

pthread_t cth[maxs];

int Socket(){
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



void* Recieve(void* args){
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

	while(True){

 		int x=recvmsg(unsfd, &message, MSG_DONTWAIT);

   		if(x>0){
   			control_msg = CMSG_FIRSTHDR(&message);
   			int clientfd;
   			memcpy(&clientfd, CMSG_DATA(control_msg), sizeof (int));

   			clientfds[clients]=clientfd;
   			
   			printf("Server provided Descriptor - %d\n", clientfd);

   			int* ffd=&clientfd;
   			
   			clients++;

   		}
   	}

}

int c=0;
pthread_t fd_recv[maxs];


void init(){
	struct SHM tmp;
	shmid=shmget(shmkey, sizeof tmp, IPC_CREAT|0666);
	shared=shmat(shmid, NULL, 0);

	shared->pid[ID]=getpid();

	int x=ID;
	sprintf(SOCKET_PATH, "%s%d.out", "a",x);

	strcpy(shared->file[ID], SOCKET_PATH);
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



void handle(){
	True=0;

	printf("server shutdown..\nstarting backup server \n");
	int i=0;
	for(i=1; i<clients; i++){
		int* arg=&clientfds[i];
		pthread_create(&cth[i], NULL, service, (void*)arg);
		//send_sfd(USFD, fds[i]);
	}

	// mmfd.unixsend=USFD;
	// mmfd.sockacc=fds[0];
	// struct FD* mfd=&mmfd;
	// pthread_create(&acc, NULL, accepter, (void*)mfd);
}

int main(int argc, char* argv[]){
	True=1;
	//server is working
	init();

	signal(SIGUSR1, handle);

	int usfd=Socket();

	int cl;
	int unsfd=accept(usfd, (struct sockaddr*)&client, &cl);

	assert(unsfd>=0);
		
	int* arg=&unsfd;
   	pthread_create(&fd_recv[0], NULL, Recieve, (void*)arg);
   	
   	while(True){
   		//do not start as server is still online
   	}
   	//server is offline
	while(1){
		int cl=0;

		int nsfd=accept(clientfds[0], (struct sockaddr*)&client, &cl);

		//snd=send_sfd(usfd, nsfd);

		clientfds[clients]=nsfd;
		int* arg=&clientfds[clients];
		pthread_create(&cth[clients], NULL, service, (void*)arg);

		clients++;
		//assert(snd>=0);
	}

	return 0;
}