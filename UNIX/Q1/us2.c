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
#define null NULL
char sp[512];
struct sockaddr_un saddr,caddr;
int fd;
int Socket(){
	int usfd=socket(AF_UNIX,SOCK_STREAM,0);
	assert(usfd>=0);
	saddr.sun_family=AF_UNIX;
	memcpy(&saddr.sun_path,sp, strlen(sp));
	unlink(sp);
	int st=bind(usfd,(struct sockaddr*)&saddr,sizeof(saddr));
	assert(st>=0);
	listen(usfd,10);
	return usfd;
}
int Send(int usfd,int nsfd){
	//send the fd to given process
	struct msghdr message;
	struct iovec iov;
	struct cmsghdr *control_msg=null;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];

	memset(&message,0,sizeof(struct msghdr));
	memset(ctrl_buf,0,CMSG_SPACE(sizeof(int)));

	data[0]=' ';
	iov.iov_base=data;
	iov.iov_len=sizeof(data);

	message.msg_name=null;
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

void Recieve(int unsfd){
	//recieve a message , here fd
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


	int x=recvmsg(unsfd, &message, 0);

	if(x>0){
		control_msg = CMSG_FIRSTHDR(&message);
		int clientfd;
		memcpy(&clientfd, CMSG_DATA(control_msg), sizeof (int));
		fd=clientfd;
	}
}

int main(int argc, char const *argv[])
{
	strcpy(sp,argv[1]);
	int usfd=Socket();
	int cl;
	int unsfd=accept(usfd,(struct sockaddr*)&caddr,&cl);
	int x;
	char buff[512];
	while(1){
		Recieve(unsfd);
		scanf("%d", &x);
		int l=read(fd, buff, x);
		write(1, buff, l);
		Send(unsfd, fd);
	}
	return 0;
}