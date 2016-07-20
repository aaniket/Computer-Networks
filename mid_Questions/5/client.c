#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#define family AF_INET
#define typ SOCK_STREAM
#define portno 9000
int main()
{
	int sfd=socket(family,typ,0);
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=family;
	serveraddr.sin_port=htons(portno);
	if(inet_aton(("127.0.0.1"),&serveraddr.sin_addr)==0)
	{
		printf("server ip error\n");
	}
	int flag=connect(sfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	assert(flag>=0);
	while(1)
	{
		char buf[30];
		read(sfd,buf,sizeof(buf));
		write(1,buf,sizeof(buf));
	}
	return 0;
}