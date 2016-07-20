#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <assert.h>

#define BUFF 4096

struct sockaddr_in dest;
int rsfd;
int len;

int createRawSocket(int protocol_num){
	int s=socket(AF_INET, SOCK_RAW, protocol_num);
	
	assert(s!=-1);

		int one=1;
	    const int *val = &one;
		int res=setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one));
	    assert(res!=-1);

	return s;
}

void createPacket(char* datagram, char* message, char* optional){
	char source_ip[32], dest_ip[32], *data, *opt;
	memset(datagram, 0, BUFF);

	struct iphdr *iph=(struct iphdr*)datagram;

	int l=strlen(optional);
	while((l%4)!=0){
		optional[l]='#';
		l++;
	}
	optional[l]='\0';
	opt=datagram+sizeof (struct iphdr);
	strcpy(opt, optional);

	data=datagram+sizeof(struct iphdr)+strlen(optional);
	strcpy(data, message);

	strcpy(source_ip, "129.0.0.1");
	strcpy(dest_ip, "127.0.0.1");

	dest.sin_family = AF_INET;
    dest.sin_port = htons(5555);
    dest.sin_addr.s_addr = inet_addr (dest_ip);

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + strlen(data) + strlen(optional);
    len=iph->tot_len;
    //iph->id = htonl (54321);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = 55;
    //iph->check = 0;
    iph->saddr = inet_addr (source_ip);
    iph->daddr = dest.sin_addr.s_addr;

}

void sendPacket(char datagram[BUFF]){
	while(1){
    	int status=sendto(rsfd, datagram, len, 0, (struct sockaddr*)&dest, sizeof dest);
    	assert(status!=-1);
    	sleep(5);
    }
}

int main(){
	rsfd=createRawSocket(55);
    
    char msg[25];
    sprintf(msg, "%s", "This is message!!");
    int l=strlen(msg);
    msg[l]='\0';

    char optional[25];
    sprintf(optional, "%s", "This is optional!!");
   	l=strlen(optional);
    optional[l]='\0';

    char datagram[BUFF];
    createPacket(datagram, msg, optional);

    sendPacket(datagram);

    return 0;
}