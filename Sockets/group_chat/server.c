#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "semaphore.c"
#include <assert.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/select.h>

#define mports 5;

struct sockaddr_in server_addr, client_addr;
int main(){
	// char *buf={"./g1","./g2","./g3","./g4"};
	int sfd,nsfd,port,clength,cnt=0;
	int port1=5049;
	int potrno[mports];
	int connected[mports];
	int pstart=5050;
	struct sockaddr_in s_addr,c_addr;
	char buffer[1024];
	for(int i=0;i<mports;i++){
		connected[i]=0;
		potrno[i]=pstart;
		pstart++;
	}
	sfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sfd>=0);
 
	s_addr.sin_family=AF_INET;
 	//s_addr.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY binds to all availible interfaces
 	s_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
 	s_addr.sin_port=htons(port1);

 	bind(sfd,(struct sockaddr_in *)&s_addr ,sizeof(s_addr));
 	listen(sfd,10);
 	printf("Not connected to any client ...\n");
 	while(1){
 		clength=sizeof(c_addr);
 		nsfd=accept(sfd,(struct sockaddr *) &c_addr,&clength);
 		printf("connected to client %d \n", cnt);
 		n=read(nsfd,buffer,1023);
 		//printf("message form client %d is %s\n",cnt,buffer);
 		int request=int(buffer[0]);
 		if(connected[request]==1){
 			printf("Already existing group..sending connect request from...\nclient %d connect request sent to group %d ",cnt,request);
 			char port[10];
 			sprintf(port,"%d",potrno[request]);
 			write(nsfd,port,sizeof(port));
 		}
 		else{
 			printf("New group..sending create request from...\nclient %d create request sent to group %d ",cnt,request);
 			char port[10];
 			sprintf(port,"%d",potrno[request]);
 			write(nsfd,port,sizeof(port));
 			//run the service
			char* pth=getenv("PWD");
			char path[100];
			sprintf(path,"%s/%s",pth,request);
			printf("%s\n",path);
			printf("forked\n");
			char* dummy;
			execl(path,dummy,NULL);
			printf("here\n");

 		}
 		//n=write(nsfd,"Message Recieved",16);
 		cnt++;
 	}

	return 0;
}