#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int rsfd;
#define BUFF 4096
void printval(int n){
	int i;
	printf("+");
	for(i=0;i<=n;i++){
		printf("=");
	}
	
}
void printval1(int n,int x){
	int i;
	printf("");
	for(i=0;i<n/2;i++){
			printf(" ");
	}
	printf("%d",x );
	for(i=0;i<n/2;i++){
		printf(" ");
	}
	printf(" |");
}
int createRawSocket(int protocol_num){
	int s=socket(AF_INET, SOCK_RAW, protocol_num);
	
	assert(s!=-1);

	return s;
}

void process(char datagram[BUFF]){

	struct sockaddr_in source,dest;

	struct iphdr* iph=(struct iphdr*)datagram;

	memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

	printf("Version %d:\n", iph->version);
	printf("IP Header Length %d words:\n", (unsigned int)iph->ihl);
	printf("Type of service: %d:\n",(unsigned int)iph->tos);
	printf("Total Length: %d:\n",ntohs(iph->tot_len));
	printf("Identification Number: %d\n",(unsigned int)iph->id);
	printf("Fragment Offset: %d\n",(unsigned int)iph->frag_off);
	printf("Time to live: %d\n",(unsigned int)iph->ttl);
	printf("Protocol: %d\n",(unsigned int)iph->protocol);
	printf("Checksum: %d\n",ntohs(iph->check));
	printf("Source IP: %s\n",inet_ntoa(source.sin_addr));
	printf("Dest IP: %s\n",inet_ntoa(dest.sin_addr));

	//Trying to print as it is
	//----------------------------------------------------------------

	printval(4);printval(4);printval(8);printval(16);printval(0);
	printf("\n");printval1(4,4);printval1(4,(unsigned int)iph->ihl);
	printval(16);printval(1);printval(1);printval(1);printval(13);
	printf("\n");
	printval(8);printval(8);printval(16);printval(1);
	printf("\n");printf("\n");
	printval(32);printval(3);
	printf("\n");printf("\n");
	printval(32);printval(3);


	//-----------------------------------------------------------------
	int len=iph->ihl;
	len-=5;

	if(len>0){
		printf("Optional\n");
		char* opt=datagram+sizeof(struct iphdr);
		len=len*4;
		while(len){
			printf("%c", (*opt));
			opt++;
			len--;
		}
		printf("\n");
	}

	char* data;
	data=datagram+(iph->ihl)*4;
	while((*data)!='\0'){
		printf("%c", (*data));
		data++;
	}
	printf("\n");
}

void receivePacket(){
	char datagram[BUFF];

	struct sockaddr_in recv;
	while(1){
		memset(datagram, 0, BUFF);
		int len;
		recvfrom(rsfd, datagram, sizeof datagram, 0, (struct sockaddr*)&recv, &len);

		printf("Received\n");

		process(datagram);
	}
}

int main(){
	rsfd=createRawSocket(55);

	receivePacket();
}