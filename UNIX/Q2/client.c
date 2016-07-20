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

struct sockaddr_in serv;

int main(int argc, char* argv[]){
	int sfd=socket(AF_INET, SOCK_STREAM, 0);

	assert(sfd>=0);

	int port=atoi(argv[1]);

	serv.sin_family=AF_INET;
	serv.sin_port=htons(port);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	int res=connect(sfd, (struct sockaddr*)&serv, sizeof serv);

	assert(res>=0);

	char buff[512];
	while(1){
		int l=read(0, buff, sizeof buff);
		write(sfd, buff, l);
		l=read(sfd, buff, l);
		write(1, buff, l);
		printf("\n");
	}
	

	return 0;
}