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

//station master

#define shmkey 555
#define pnum 2
#define BUFF 4096

struct SHM{
	int pid;
}shm;

struct SHM *shared;

int shmid;
int cpid[pnum];
int freep;
int busy[pnum];
struct sockaddr_un addr;
int slen;

void handle(){
	int id=shared->pid;
	int i;
	for(i=0; i<pnum; i++){
		if(id==cpid[i]){
			busy[i]=0;
			freep++;
			break;
		}
	}
}

void init(){
	shmid=shmget(shmkey, sizeof shm,0666);
	shared=shmat(shmid, NULL, 0);
	freep=3;
}

int sfd, usfd[pnum], rsfd;

struct sockaddr_in serv;

int createSocket(int port){
	int fd=socket(AF_INET, SOCK_STREAM, 0);
	assert(fd!=-1);

	serv.sin_family=AF_INET;
	serv.sin_port=htons(port);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	int r=bind(fd, (struct sockaddr*)&serv, sizeof serv);

	assert(r!=-1);

	listen(fd, 10);
	return fd;
}

int connect_to_unix(char SOCKET_PATH[128]){
	int usfd=socket(AF_UNIX, SOCK_STREAM, 0);

	assert(usfd>=0);

	addr.sun_family=AF_UNIX;
	memcpy(&addr.sun_path, SOCKET_PATH, strlen(SOCKET_PATH));

	int res=connect(usfd, (struct sockaddr*)&addr, sizeof addr);

	assert(res>=0);

	printf("%s\n", "connected");
	return usfd;
}

int send_sfd(int usfd, int nsfd){
	struct msghdr message;
	struct iovec iov;
	struct cmsghdr *control_msg=NULL;
	char ctrl_buf[CMSG_SPACE(sizeof (int))];
	char data[1];

	memset(&message, 0, sizeof (struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof (int)));

	//passing dummy message so that recvmsg doesnt return 0
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

void announce(char message[128]){
	struct sockaddr_in dest;

    char datagram[4096], source_ip[32], dest_ip[32], *data;
	memset(datagram, 0, BUFF);

	struct iphdr *iph=(struct iphdr*)datagram;

	data=datagram+sizeof(struct iphdr);
	strcpy(data, message);

	strcpy(source_ip, "127.0.0.1");
	strcpy(dest_ip, "127.0.0.1");

	dest.sin_family = AF_INET;
    dest.sin_port = htons(5555);
    dest.sin_addr.s_addr = inet_addr (dest_ip);

    slen=sizeof(struct iphdr)+strlen(message);
   	int status=sendto(rsfd, datagram, slen, 0, (struct sockaddr*)&dest, sizeof dest);
    assert(status!=-1);
}

void start(){
	while(1){
		while(freep==0);
		struct sockaddr_in cl;
		int len;
		int nsfd=accept(sfd, (struct sockaddr*)&cl, &len);
			
		printf("%d accepted\n", nsfd);

		char message[128];
		strcpy(message, "Train is arriving");
		int l=strlen(message);
		message[l]='\0';
		
		//announce(message);

		int i=0;
		for(i=0; i<pnum; i++){
			if(busy[i]==0){
				int snd=send_sfd(usfd[i], nsfd);

				printf("sent\n");
				assert(snd!=-1);
				busy[i]=1;
				break;
			}
		}
	}
}

int createRawSocket(int protocol_num){
	int s=socket(AF_INET, SOCK_RAW, protocol_num);
	
	assert(s!=-1);

	// int one=1;
	// const int *val = &one;
	// int res=setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one));
	// assert(res!=-1);

	return s;
}

int main(int argc, char* argv[]){
	signal(SIGUSR1, handle);

	printf("pid: %d\n", getpid());
	
	init();
	
	int i;
	for(i=0; i<pnum; i++)
		cpid[i]=atoi(argv[2+pnum+i]);

	for(i=0; i<pnum; i++)
		usfd[i]=connect_to_unix(argv[2+i]);

	sfd=createSocket(atoi(argv[1]));
	
	rsfd=createRawSocket(55);

	start();
}