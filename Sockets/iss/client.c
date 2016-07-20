#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>

#define BUFFSIZE 105
#define maxs 105

int sfd;
int port[3]={0, 5000, 6000};

int service;

struct sockaddr_in server_addr;
int main(){
	
	scanf("%d", &service);

	int sfd=socket(AF_INET, SOCK_STREAM, 0);
	assert(sfd>=0);

	server_addr.sin_port=htons(port[service]);
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

	int status=connect(sfd, (struct sockaddr*)&server_addr, sizeof server_addr);
	assert(status>=0);

	int a,b;
	scanf("%d %d", &a, &b);

	char inp[BUFFSIZE];
	sprintf(inp, "%d %d", a, b);

	send(sfd, inp, sizeof inp, 0);
	
	char ans[BUFFSIZE];
	recv(sfd, ans, sizeof ans, 0);
	printf("The ans is %s\n", ans);
	return 0;
}