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
int gsfd;

void *readfn()
{

	while(1){
	char buf1[300];
	int n=read(0,buf1,sizeof(buf1));
	//printf("input is %s\n",buf1 );
	buf1[n]='\0';
	write(gsfd,buf1,sizeof(buf1));
	}
}

void *writefn()
{
	char buf[30];
	char filename1[40];
	sprintf(filename1,"%s%s",pd,"famousfifo1.txt");
	int ffd1=open(filename1,0666);
	read(ffd1,buf,sizeof(buf));
	printf("%s",buf);
}


//struct sockaddr_in server_addr, client_addr;
int main(){
	int sfd,port,n;
 	struct sockaddr_in s_addr,g_addr;
 	struct hostent *server;
 	//hostent used to store information for a host
 	char buffer[1024];
 	///port=atoi(argv[2]);
 	sfd=socket(AF_INET, SOCK_STREAM ,0);
 	//server=gethostbyname(argv[1]);
 	port=5049;
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
 	printf("Enter the group no. you want to connect to\n");
 	//bzero(buffer,1024);
 	fgets(buffer,1023,stdin);
 	//scanf("%s",&buffer);
 	n=write(sfd,buffer,strlen(buffer));
 	printf("request sent \n");
 	bzero(buffer,1024);
 	n=read(sfd,buffer,1023);
 	printf("port id recieved- %s\n",buffer);


 	gsfd=socket(AF_INET, SOCK_STREAM ,0);
 	g_addr.sin_family=AF_INET;
 	
 	toint(buffer,n);

   	g_addr.sin_port = htons(port);
   	g_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
   	x=connect(gsfd,(struct sockaddr *)&g_addr,sizeof(g_addr));
   	printf("connected to the group you can start chatting now\n");

	pthread_create(&rdthread,NULL,readfn,NULL);
	pthread_create(&wrthread,NULL,writefn,NULL);
 	return 0;
}