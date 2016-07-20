#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define max_clients 5;
 int main(int argc, char *argv[]){
 	int sfd,port,n;
 	struct sockaddr_in s_addr;
 	struct hostent *server;
 	//hostent used to store information for a host
 	char buffer[1024];
 	///port=atoi(argv[2]);
 	sfd=socket(AF_INET, SOCK_STREAM ,0);
 	//server=gethostbyname(argv[1]);
 	port=5002;
 	//returns hostent struct for given hostname 
 	//name can be host name or IPv4 address
 	//bzero((char *) &s_addr,sizeof(s_addr)); 
 	s_addr.sin_family=AF_INET;
 	//bcopy((char *)s_addr.h_addr, (char *)&s_addr.sin_addr.s_addr, s_addr.h_length);
   	//copies n bytes form s1 to s2 bcopy(void * s1,void *s2,int n)
   	s_addr.sin_port = htons(port);
   	s_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
 	//connect to the server
 	int x=connect(sfd,(struct sockaddr *)&s_addr,sizeof(s_addr));
 	
 	printf("x %d\n",x);
 	printf("Enter the message\n");
 	//bzero(buffer,1024);
 	fgets(buffer,1023,stdin);
 	//scanf("%s",&buffer);
 	n=write(sfd,buffer,strlen(buffer));
 	printf("Message has been weitten \n");
 	bzero(buffer,1024);
 	n=read(sfd,buffer,1023);
 	printf("server response - %s\n",buffer);
 	return 0;
 }
