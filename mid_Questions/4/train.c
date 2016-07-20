#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include "mysem.c"
#define family AF_INET
#define typ SOCK_STREAM
int services_portno[3];
int main()
{
	services_portno[0]=9000;
	services_portno[1]=9001;
	services_portno[2]=9002;
	int sfd=socket(family,typ,0);
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=family;
	if(inet_aton("127.0.0.1",&serveraddr.sin_addr) ==0)
	{
		printf("error in address binding\n");
	}
	int n;
	printf("Enter the train no:");
	scanf("%d",&n);
	serveraddr.sin_port=htons(services_portno[n-1]);
	int flag=connect(sfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	assert(flag>=0);
	char res[2];
	int l=read(sfd,res,sizeof(res));
	res[l]='\0';
	if(res[0]=='0')
	{
		printf("platform is not available now.\n");
		return 0;
	}
	return 0;
}