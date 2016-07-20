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

struct sockaddr_un addr;
struct sockaddr_in serv, client;

char SOCKET_PATH[512];

int connect_to_unix(){
	int usfd=socket(AF_UNIX, SOCK_STREAM, 0);

	assert(usfd>=0);

	addr.sun_family=AF_UNIX;
	memcpy(&addr.sun_path, SOCKET_PATH, strlen(SOCKET_PATH));

	//printf("%s\n", SOCKET_PATH);
	int res=connect(usfd, (struct sockaddr*)&addr, sizeof addr);

	assert(res>=0);

	return usfd;
}

int create_socket(int port){
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

int main(int argc, char* argv[]){

	strcpy(SOCKET_PATH, argv[1]);
	int port=atoi(argv[2]);

	int usfd=connect_to_unix();

	int sfd=create_socket(port);

	while(1){
		int cl=0;
		int nsfd=accept(sfd, (struct sockaddr*)&client, &cl);

		int snd=send_sfd(usfd, nsfd);

		assert(snd>=0);
	}
	return 0;
}