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

#define ID 1
#define shmkey 555
#define maxs 15

struct SHM{
	int pid[10];
	char file[10][55];
};

struct SHM* shared;
int shmid;

int USFD;
int first;
void handler(){
	first++;
	if(first==2){
		raise(SIGKILL);
	}
	else{
		close(USFD);
		int p=shared->pid[ID+1];
		kill(p, SIGUSR1);
	}
}

struct sockaddr_un addr;
struct sockaddr_in serv, client;

char SOCKET_PATH[512];

int Connect(){
	int usfd=socket(AF_UNIX, SOCK_STREAM, 0);

	assert(usfd>=0);

	addr.sun_family=AF_UNIX;
	memcpy(&addr.sun_path, SOCKET_PATH, strlen(SOCKET_PATH));

	//printf("%s\n", SOCKET_PATH);
	int res=connect(usfd, (struct sockaddr*)&addr, sizeof addr);

	assert(res>=0);

	return usfd;
}

int Socket(int port){
	int sfd=socket(AF_INET, SOCK_STREAM, 0);

	assert(sfd>=0);

	serv.sin_family=AF_INET;
	serv.sin_port=htons(port);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");

	int st=bind(sfd, (struct sockaddr*)&serv, sizeof serv);

	listen(sfd, 10);

	assert(st>=0);

	return sfd;
}

int Send(int usfd, int nsfd){
	struct msghdr message;
	struct iovec iov;
	struct cmsghdr *control_msg=NULL;
	char ctrl_buf[CMSG_SPACE(sizeof (int))];
	char data[1];

	memset(&message, 0, sizeof (struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof (int)));
	data[0]=' ';
	iov.iov_base=data;
	iov.iov_len=sizeof (data);

	message.msg_name=NULL;
	message.msg_namelen=0;
	message.msg_iov=&iov;
	message.msg_iovlen=1;
	message.msg_controllen=CMSG_SPACE(sizeof(int));
	message.msg_control=ctrl_buf;

	control_msg=CMSG_FIRSTHDR(&message);
	control_msg->cmsg_level=SOL_SOCKET;
	control_msg->cmsg_type=SCM_RIGHTS;

	memcpy(CMSG_DATA(control_msg),&nsfd,sizeof(nsfd));

	
	control_msg->cmsg_len = CMSG_LEN(sizeof(int));


	int snd=sendmsg(usfd, &message, 0);

	return snd;
}

void init(){
	struct SHM tmp;
	shmid=shmget(shmkey, sizeof tmp, IPC_CREAT|0666);
	shared=shmat(shmid, NULL, 0);

}

void* service(void* args){
	int* arg=(int*)args;
	int fd=*arg;

	char buff[512];
	while(1){
		int l=read(fd, buff, sizeof buff);
		int i;
		int sum=0;
		for(i=0; i<l; i++){
			sum+=(int)buff[i];
		}
		// char buf1[1024];
		// sprintf(buf1,"%d",sum);
		write(fd, buff, sizeof(buff));
	}
}

pthread_t cth[maxs];
int c=0;
int fds[maxs];

int main(int argc, char* argv[]){
	init();

	signal(SIGINT, handler);

	strcpy(SOCKET_PATH, shared->file[ID+1]);
	int port=atoi(argv[1]);

	int usfd=Connect();
	USFD=usfd;

	int sfd=Socket(port);

	int snd=Send(usfd, sfd);
	printf("sfd %d\n", sfd);
	assert(snd>=0);

	while(1){
		int cl=0;
		int nsfd=accept(sfd, (struct sockaddr*)&client, &cl);

		snd=Send(usfd, nsfd);

		fds[c]=nsfd;
		int* arg=&fds[c];
		pthread_create(&cth[c], NULL, service, (void*)arg);

		c++;
		assert(snd>=0);
	}
	return 0;
}