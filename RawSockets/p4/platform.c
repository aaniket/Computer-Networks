#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

#define BUFF 4096
#define shmkey 555

//platform

struct SHM{
	int pid;
}shm;

struct SHM* shared;

int shmid;

struct MSG{
	int type;
	char msg[256];
}msg;

char SOCKET_PATH[128];
int usfd, rsfd_a, rsfd_c;
struct sockaddr_un addr;
//int rsfd;

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

int createRawSocket(int protocol_num){
	int s=socket(AF_INET, SOCK_RAW, protocol_num);
	
	assert(s!=-1);

	return s;
}

void* recv_announce(void* arg){
	int* args=(int*)arg;
	int fd=*args;

	char datagram[4096];

	struct sockaddr_in recv;
	while(1){
		memset(datagram, 0, BUFF);
		int len;
		recvfrom(fd, datagram, sizeof datagram, 0, (struct sockaddr*)&recv, &len);

		printf("Announcment: ");
		char* data=datagram+sizeof (struct iphdr);
		while((*data)!='\0'){
			printf("%c", *data);
			data++;
		}
		printf("\n");
	}
}

void* recv_cable(void* arg){
	int* args=(int*)arg;
	int fd=*args;

	char datagram[4096];

	struct sockaddr_in recv;
	while(1){
		memset(datagram, 0, BUFF);
		int len;
		recvfrom(fd, datagram, sizeof datagram, 0, (struct sockaddr*)&recv, &len);

		printf("Advertisement: ");
		char* data=datagram+sizeof (struct iphdr);
		while((*data)!='\0'){
			printf("%c", *data);
			data++;
		}
		printf("\n");
	}
}

int recv_fd(int uunsfd){

	struct msghdr message;
	struct iovec iov;
	struct cmsghdr *control_msg = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];
	
	memset(&message, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

	data[0]=' ';
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
		int x=recvmsg(uunsfd, &message, 0);	

   		if(x>=0){
   			printf("received\n");

   			control_msg = CMSG_FIRSTHDR(&message);
   			int clientfd;
   			memcpy(&clientfd, CMSG_DATA(control_msg), sizeof (int));
   		
   			return clientfd;
   		}
	}
}

pthread_t ann, cab;

int parent_id;

void inform(){
	shared->pid=getpid();
	kill(parent_id, SIGUSR1);
}

void init(){
	shmid=shmget(shmkey, sizeof shm, IPC_CREAT|0666);
	shared=shmat(shmid, NULL, 0);
}

int main(int argc, char* argv[]){
	init();

	printf("pid: %d\n", getpid());

	strcpy(SOCKET_PATH, argv[1]);

	usfd=create_unix_socket();

	int cl;
	struct sockaddr_in client;
	int unsfd=accept(usfd, (struct sockaddr*)&client, &cl);

	assert(usfd!=-1);

	rsfd_a=createRawSocket(55);
	rsfd_c=createRawSocket(65);

	int *tmp=&rsfd_a;
	pthread_create(&ann, NULL, recv_announce, (void*)tmp);

	int *tmp1=&rsfd_c;
	pthread_create(&cab, NULL, recv_cable, (void*)tmp1);

	scanf("%d", &parent_id);

	while(1){
		int cfd=recv_fd(unsfd);

		assert(cfd!=-1);

		while(1){
			int l=read(cfd, &msg, sizeof msg);
			if(msg.type==0){
				inform();
				break;
			}
			printf("%s\n", msg.msg);
		}
	}

	return 0;
}