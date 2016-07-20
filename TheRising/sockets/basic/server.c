#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <assert.h>

int main(){
	struct sockaddr_in saddr,caddr;
	int n,ret,nsfd,sfd;
	char buffer[1024];
	int length;

	sfd=socket(AF_INET,SOCK_STREAM,0);

	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(5002);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	bind(sfd,(struct sockaddr *) &saddr,sizeof(saddr));
	//assert(ret>=0);

	listen(sfd,2);

	while(1){
		printf("started fucking me...\n");
		length=sizeof(caddr);
		nsfd=accept(sfd,(struct sockaddr*)&caddr,&length);
		printf("Connection accepted\n");
		read(nsfd,buffer,1023);
		printf(">client:%s\n",buffer);
		fgets(buffer,1023,stdin);
		write(nsfd,buffer,strlen(buffer));
	}
	return 0;

}