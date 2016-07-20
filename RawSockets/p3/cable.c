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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>


#define BUFF 4096

//cable
int rsfd;

void advertise(char message[128]){
	struct sockaddr_in dest;

    char datagram[4096], source_ip[32], dest_ip[32], *data;
	memset(datagram, 0, BUFF);

	struct iphdr *iph=(struct iphdr*)datagram;

	data=datagram+sizeof(struct iphdr);
	strcpy(data, message);

	strcpy(source_ip, "127.0.0.1");
	strcpy(dest_ip, "127.0.0.1");

	dest.sin_family = AF_INET;
    dest.sin_port = htons(5555);
    dest.sin_addr.s_addr = inet_addr (dest_ip);

    int slen=sizeof (struct iphdr)+strlen(message);
   	int status=sendto(rsfd, datagram, slen, 0, (struct sockaddr*)&dest, sizeof dest);
    assert(status!=-1);
}

int createRawSocket(int protocol_num){
	int s=socket(AF_INET, SOCK_RAW, protocol_num);
	
	assert(s!=-1);

	// int one=1;
	// const int *val = &one;
	// int res=setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one));
	// assert(res!=-1);

	return s;
}

int main(){
	rsfd=createRawSocket(65);

	char message[128];
	while(1){
		scanf("%s", message);
		advertise(message);
	}
}