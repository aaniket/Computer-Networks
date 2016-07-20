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

int main(void)
{
	int sfd;
	struct sockaddr_in saddr;
	char packet[50];
	struct iphdr *ip = (struct iphdr *)packet;

	if ((sfd = socket(AF_INET, SOCK_RAW, 55)) < 0)
	{
		perror("error:");
		exit(EXIT_FAILURE);
	}

	memset(packet, 0, sizeof(packet));
	int fromlen = sizeof(saddr);

	while(1)
	{
		recvfrom(sfd, packet, sizeof(packet), 0,(struct sockaddr *)&saddr, &fromlen);
		char * data=packet+sizeof(struct iphdr);
		while((*data)!='\0' && (*data)!='\n'){
			printf("%c", *data);
			data++;
		}
		printf("\n");
		print_iphdr((struct iphdr *)packet);
	}
}

