#include <pcap.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <assert.h>

#define B_SZ 65536
#define SZ 1024
int linkhdrlen;

void parse_packet(u_char *user, struct pcap_pkthdr *packethdr, 
                  u_char *packetptr)
{
    struct ip* iphdr;
    struct icmphdr* icmphdr;
    struct tcphdr* tcphdr;
    struct udphdr* udphdr;
    char iphdrInfo[256], srcip[256], dstip[256];
    unsigned short id, seq;
 
    // Skip the datalink layer header and get the IP header fields.
    packetptr += linkhdrlen;
    iphdr = (struct ip*)packetptr;
    strcpy(srcip, inet_ntoa(iphdr->ip_src));
    strcpy(dstip, inet_ntoa(iphdr->ip_dst));
    sprintf(iphdrInfo, "ID:%d TOS:0x%x, TTL:%d IpLen:%d DgLen:%d",
            ntohs(iphdr->ip_id), iphdr->ip_tos, iphdr->ip_ttl,
            4*iphdr->ip_hl, ntohs(iphdr->ip_len));
 
    // Advance to the transport layer header then parse and display
    // the fields based on the type of hearder: tcp, udp or icmp.
    packetptr += 4*iphdr->ip_hl;
    switch (iphdr->ip_p)
    {
    case IPPROTO_TCP:
        tcphdr = (struct tcphdr*)packetptr;
        printf("TCP  %s:%d -> %s:%d\n", srcip, ntohs(tcphdr->source),
               dstip, ntohs(tcphdr->dest));
        printf("%s\n", iphdrInfo);
        printf("%c%c%c%c%c%c Seq: 0x%x Ack: 0x%x Win: 0x%x TcpLen: %d\n",
               (tcphdr->urg ? 'U' : '*'),
               (tcphdr->ack ? 'A' : '*'),
               (tcphdr->psh ? 'P' : '*'),
               (tcphdr->rst ? 'R' : '*'),
               (tcphdr->syn ? 'S' : '*'),
               (tcphdr->fin ? 'F' : '*'),
               char* data=(char *)((unsigned char *)tcphdr + (tcphdr->doff * 4));
               printf("%s\n", data);
               ntohl(tcphdr->seq), ntohl(tcphdr->ack_seq),
               ntohs(tcphdr->window), 4*tcphdr->doff);
        break;
 
    case IPPROTO_UDP:
        udphdr = (struct udphdr*)packetptr;
        printf("UDP  %s:%d -> %s:%d\n", srcip, ntohs(udphdr->source),
               dstip, ntohs(udphdr->dest));
        printf("%s\n", iphdrInfo);
        break;
 
    case IPPROTO_ICMP:
        icmphdr = (struct icmphdr*)packetptr;
        printf("ICMP %s -> %s\n", srcip, dstip);
        printf("%s\n", iphdrInfo);
        memcpy(&id, (u_char*)icmphdr+4, 2);
        memcpy(&seq, (u_char*)icmphdr+6, 2);
        printf("Type:%d Code:%d ID:%d Seq:%d\n", icmphdr->type, icmphdr->code,ntohs(id), ntohs(seq));
      
        break;
    }
    printf(
        "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
}


void another_callback(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet) { 
	int i=0; 
	static int count=0; 

	// printf("Packet Count: %d\n", ++count);   
	// printf("Recieved Packet Size: %d\n", pkthdr->len);    
	// printf("Payload:\n");                    
	// for(i=0;i<pkthdr->len;i++) { 
	// 	if(isprint(packet[i]))               
	// 		printf("%c ",packet[i]);         
	// 	else
	// 		printf(" . ",packet[i]);         
	// 	if((i%16==0 && i!=0) || i==pkthdr->len-1) 
	// 		printf("\n"); 
	// }
	parse_packet(arg,pkthdr,packet)
}


void callback(u_char *args,const struct pcap_pkthdr* pkthdr,const u_char* packet){
	static int count=1;
	fprintf(stdout, "%3d ",count);
	fflush(stdout);
	//another_callback(args,pkthdr,packet);
	count++;
}

int main(int argc, char const *argv[])
{
	 char interface[256] = "", bpfstr[256] = "";
    int packets = 0, c, i;
 
    // Get the command line options, if any
    while ((c = getopt (argc, argv, "hi:n:")) != -1)
    {
        switch (c)
        {
        case 'h':
            printf("usage: %s [-h] [-i ] [-n ] []\n", argv[0]);
            exit(0);
            break;
        case 'i':
            strcpy(interface, optarg);
            break;
        case 'n':
            packets = atoi(optarg);
            break;
        }
    }
 
    // Get the packet capture filter expression, if any.
    for (i = optind; i < argc; i++)
    {
        strcat(bpfstr, argv[i]);
        strcat(bpfstr, " ");
    }
 
    // Open libpcap, set the program termination signals then start
    // processing packets.
    if ((pd = open_pcap_socket(interface, bpfstr)))
    {
        signal(SIGINT, bailout);
        signal(SIGTERM, bailout);
        signal(SIGQUIT, bailout);
        capture_loop(pd, packets, (pcap_handler)parse_packet);
        bailout(0);
    }
//     pcap_loop(descr,-1,another_callback,NULL);


	return 0;
}