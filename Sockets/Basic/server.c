#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
//cont size_t max_clients=5;
 int main(int argc, char *argv[]){
 	
 	int sfd,nsfd,port,clength,cnt=0;
 	char buffer[1024];
 	struct sockaddr_in s_addr,c_addr;
 	int n;
 	//socket establishing server
 	sfd=socket(AF_INET,SOCK_STREAM,0);
 	//socket(family,type,protocol)
 	//bzero((char *) &s_addr,sizeof(s_addr));
 	//bzero(void *s,size_t n) --> puts first n bytes=0
 	//in area pointed by s
 	port=5002;

 	s_addr.sin_family=AF_INET;
 	//s_addr.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY binds to all availible interfaces
 	s_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
 	s_addr.sin_port=htons(port);
 	//htons/l host to network short/long same for ntohs/l
 	//because ordering of mutibyte integer can be different on different machines

 	//binding address
 	bind(sfd,(struct sockaddr_in *)&s_addr ,sizeof(s_addr));
 	//listen(sfd, max_clients)
 	listen(sfd,2);
 	while(1){
 		printf("Not connected to any client ...\n");
 		clength=sizeof(c_addr);
 		nsfd=accept(sfd,(struct sockaddr *) &c_addr,&clength);
 		printf("connected to client %d \n", cnt);
 		//bzero(buffer,1024);
 		n=read(nsfd,buffer,1023);
 		printf("message form client %d is %s\n",cnt,buffer);
 		n=write(nsfd,"Message Recieved",16);
 		cnt++;
 	}
 	return 0;
 }
