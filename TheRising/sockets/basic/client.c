#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <assert.h>
#include <sys/socket.h>

int main(){
	int nsfd,n,length;
	struct sockaddr_in saddr;
	char buffer[1024];

	nsfd=socket(AF_INET,SOCK_STREAM,0);
	//assert(nsfd>=0);

	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(5002);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	int ret=connect(nsfd,(struct sockaddr*)&saddr,sizeof(saddr));
	while(1){
		printf(">client:");
		fgets(buffer,1023,stdin);
		write(nsfd,buffer,strlen(buffer));
		read(nsfd,buffer,1023);
		printf(">server:%s\n",buffer);
	}
	return 0;
}