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

#define DEST "127.0.0.1"
#define SOURCE "127.0.0.1"

int main(void)
{

	int sfd;
	struct sockaddr_in daddr,saddr;
	char packet[50];

	/* point the iphdr to the beginning of the packet */
	struct iphdr *ip = (struct iphdr *)packet;

	if ((sfd = socket(AF_INET, SOCK_RAW, 55)) < 0) {
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
	ip->protocol = 55;	/* protocol at L4 */
	ip->check = 0;			/* not needed in iphdr */
	ip->saddr = daddr.sin_addr.s_addr;
	ip->daddr = daddr.sin_addr.s_addr;

	while(1)
	{
		//scanf("%s",packet+sizeof(struct iphdr));
		sendto(sfd, (char *)packet, sizeof(packet), 0, (struct sockaddr *)&daddr, (socklen_t)sizeof(daddr));
	}
}
