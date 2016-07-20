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

//train

int sfd;
struct sockaddr_in serv;

struct MSG{
	int type;
	char msg[256];
}msg;

int main(int argc, char* argv[]){
	sfd=socket(AF_INET, SOCK_STREAM, 0);

	assert(sfd!=-1);

	int port=atoi(argv[1]);
	serv.sin_port=htons(port);
	serv.sin_family=AF_INET;
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	int s=connect(sfd, (struct sockaddr*)&serv, sizeof serv);

	assert(s!=-1);

	int t;
	while(1){
		scanf("%d", &t);
		msg.type=t;
		if(t==1)
		scanf("%s", msg.msg);
		int snd=send(sfd, &msg, sizeof msg, 0);
		assert(snd!=-1);
		if(t==0)break;
		
	}

	while(1);
	return 0;
}