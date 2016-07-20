#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pcap.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include <assert.h>
int sfd;
#define DEST "127.0.0.1"
#define SOURCE "127.0.0.1"
char myid[3];
int sz=0;
void print_boundary(){
	int i;

	printf("\n");

	for(i=0;i<20;i++)
	printf(" ");

	for(i=0;i<17;i++)
	printf("_");

	printf("\n");

	for(i=0;i<20;i++)
	printf(" ");
}
void print_data(int num,int fact,int extra){
	printf("%d",num);

	int count=0;
	int i;
	if(num!=0)
	while(num/fact==0)
	{
		fact/=10;
		count++;
	}

	else
	while(fact!=1)
	{
		fact=fact/10;
		printf(" ");
	}

	for(i=0;i<count;i++)
	printf(" ");


	for(i=0;i<extra;i++)
	printf(" ");

	printf("|");
}




void print_ip(struct sockaddr_in s)
{
	char *ip_addr;
	int n;

	ip_addr=inet_ntoa(s.sin_addr);
	n=strlen(ip_addr);
	printf("%s",ip_addr);
	int i;
	for(i=0;i<15-n;i++)
	printf(" ");
	printf("|");
}

void print_iphdr(struct iphdr *ip)
{
	print_boundary();
	printf("|");

	print_data(ip->version,1,0);

	print_data(ip->ihl,10,0);

	print_data(ip->tos,100,0);

	print_data(ntohs(ip->tot_len),10000,1);

	print_boundary();
	printf("|");

	print_data(ip->id,10000,3);

	print_data(ip->frag_off,10000,1);

	print_boundary();
	printf("|");

	print_data(ip->ttl,100,1);

	print_data(ip->protocol,100,0);

	print_data(ip->check,10000,1);

	print_boundary();
	printf("|");

	struct sockaddr_in s;

	s.sin_addr.s_addr=ip->saddr;
	print_ip(s);

	print_boundary();
	printf("|");

	s.sin_addr.s_addr=ip->daddr;
	print_ip(s);

	print_boundary();
	printf("\n\n");
}

void recvRaw(){
	sleep(2);
	printf("in recv\n");
	int sfd2;
	struct sockaddr_in saddr;
	char packet[50];
	struct iphdr *ip = (struct iphdr *)packet;

	if ((sfd2 = socket(AF_INET, SOCK_RAW, 155)) < 0)
	{
		perror("error:");
		exit(EXIT_FAILURE);
	}

	memset(packet, 0, sizeof(packet));
	int fromlen = sizeof(saddr);
	printf("almost there\n");
		recvfrom(sfd2, packet, sizeof(packet), 0,(struct sockaddr *)&saddr, &fromlen);
		printf("almost there\n");
		printf("packet recieved %s\n",packet+sizeof(struct iphdr));
		print_iphdr((struct iphdr *)packet);
	
	 printf("complete\n");

}



int writefn()
{
	//Apti
	while(1){
		char buffer[1024],buffer1[1024];
		int n=read(sfd,buffer,sizeof(buffer));
		buffer[n]='/0';
		int val=atoi(buffer);
		
		if(val==11540){
			printf("congrats you passed the apptitude test get ready for the interview \n");
			break;
		}

		printf("Question is : ");
		printf("%s\n",buffer );
		buffer[0]='/0';
		n=read(0,buffer1,sizeof(buffer1));
		buffer1[n]=0;
		write(sfd,buffer1,sizeof(buffer1));

	}
	//Interview

	char buffer[1024],buffer1[1024];
	// int n=read(sfd,buffer1,sizeof(buffer1));
	// buffer1[n]=0;
	// myid[sz++]=buffer1[0];


	while(1){
		char buffer[1024],buffer1[1024];
		int n=read(sfd,buffer,sizeof(buffer));
		buffer[n]='/0';
		int val=atoi(buffer);
		
		if(val==11540){
			printf("congrats you passed the interview get ready for the next interview \n");
			sleep(3);
			recvRaw();
			printf("after\n");
			//recvRaw();
	
			break;
		}

		printf("Question is : ");
		printf("%s\n",buffer );
		buffer[0]='/0';
		n=read(0,buffer1,sizeof(buffer1));
		buffer1[n]=0;
		write(sfd,buffer1,sizeof(buffer1));

	}


}

//struct sockaddr_in server_addr, client_addr;
int main(int argc,char *argv[]){
	int port,n;
 	struct sockaddr_in s_addr;
 	char buffer[1024];
 	sfd=socket(AF_INET, SOCK_STREAM ,0);
 	port=atoi(argv[1]);
 	s_addr.sin_family=AF_INET;
   	s_addr.sin_port = htons(port);
   	s_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
 	//connect to the server
 	int x=connect(sfd,(struct sockaddr *)&s_addr,sizeof(s_addr));

 	pid_t pid =getpid();
 	char pids[1024];
 	sprintf(pids,"%d",pid);
 	write(sfd,pids,strlen(pids));


 
 	//fgets(buffer,1023,stdin);

	int score=writefn();


 	return 0;
}