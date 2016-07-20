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
#include <poll.h>

#define timeout 500
char buff[1024];
#define DEST "127.0.0.1"
#define SOURCE "127.0.0.1"
void sendraw(){
printf("in %s\n", buff);

	int sfd;
	struct sockaddr_in daddr,saddr;
	char packet[50];

	/* point the iphdr to the beginning of the packet */
	struct iphdr *ip = (struct iphdr *)packet;

	if ((sfd = socket(AF_INET, SOCK_RAW, 155)) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}

	// int one=1;
	// const int *val=&one;
	// setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));
	// perror("setsockopt ");

	daddr.sin_family = AF_INET;
	//daddr.sin_port = 0; /* not needed in SOCK_RAW */
	inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
	memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));

	saddr.sin_family = AF_INET;
	//saddr.sin_port = 0; /* not needed in SOCK_RAW */
	inet_pton(AF_INET, SOURCE, (struct in_addr *)&saddr.sin_addr.s_addr);
	memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));


	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = htons(40);	/* 16 byte value */
	ip->frag_off = 0;		/* no fragment */
	ip->ttl = 64;			/* default value */
	ip->protocol = 155;	/* protocol at L4 */
	ip->check = 0;			/* not needed in iphdr */
	ip->saddr = daddr.sin_addr.s_addr;
	ip->daddr = daddr.sin_addr.s_addr;
//scanf("%s",packet+sizeof(struct iphdr));
	while(1){
		sprintf(packet,"%s",buff);
		//printf("packet %s\n", packet);
		sendto(sfd, packet, sizeof(packet), 0, (struct sockaddr *)&daddr, (socklen_t)sizeof(daddr));
	}
}


int main(){
	int sfd,nsfd,port,clength,cnt=0;
	int port1=14112;
	struct sockaddr_in s_addr,c_addr;
	char buffer[1024];
	sfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sfd>=0);
	
	s_addr.sin_family=AF_INET;
 	//s_addr.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY binds to all availible interfaces
	s_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	s_addr.sin_port=htons(port1);

	bind(sfd,(struct sockaddr_in *)&s_addr ,sizeof(s_addr));

	listen(sfd,5);
	printf("Not connected to any interviever ...\n");

	struct pollfd fds[4];
	int n=0;
	while(1){
			char list[1024];
			int lval=0;
			nsfd=accept(sfd,(struct sockaddr *) &c_addr,&clength);
			fds[n].fd=nsfd;
			fds[n].events=POLLIN;
			n++;
			int ret=poll(fds,n,timeout);
			if(ret>0){
				int i=0;
				for(i=0;i<n;i++){
					if(fds[i].revents&POLLIN){
						buff[0]='/0';
						int n=read(fds[i].fd,buff,sizeof(buff));
						buff[n]='/0';
						printf("buffer %s\n",buff );
						sprintf(list,"%s",buff);
						lval+=n;
					}
				}

				list[lval]='/0';
				printf("fd number and score list \n %s \n",list );
				//sleep(3);
				sendraw();
			}
	}
	return 0;

}