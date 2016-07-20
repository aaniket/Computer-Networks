#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/signal.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include "mysem.c"
#define family AF_INET
#define typ SOCK_STREAM
#define timeout 500
int services_portno[3];
#define no_of_services 3
//server code for service co-ordinator system
struct service
{
	int sid;
	int is_alloted;
	int size;
};

struct sharedmem
{
	int serverid;
	int platformid[3];
};
int cnt=0;
struct service *s[3];
int arr[3];
int is_available()
{
	int i,fl=0;
	for(i=0;i<cnt;i++)
	{
		if(s[i]->is_alloted==0)
		{
			s[i]->is_alloted=1;
			arr[i]=1;
			return i;
		}
	}
	return -1;
}
char* rfilename[3];
char* wfilename[3];
void handler()
{
	int i;
	printf("handler function\n");
	for(i=0;i<cnt;i++)
	{
		//printf("%d %d\n",s[i]->is_alloted,arr[i]);
		if(s[i]->is_alloted==0 && arr[i]==1)
		{
			arr[i]=0;
			char buf[39];
			sprintf(buf,"%s %d","train has been left from platform no ",i+1);
			int len=strlen(buf);
			buf[len]='\0';
			write(1,buf,sizeof(buf));
			break;
		}
	}
	signal(SIGUSR1,handler);
}
int main(int argc,char* argv[])
{
	struct sharedmem* shared;
	key_t key = ftok(".",3429);
	int shmid=shmget(key,1024,IPC_CREAT|0666);
	shared=(struct sharedmem*)shmat(shmid,NULL,0);
	shared->serverid=(int)getpid();
	//printf("%d\n",shared->serverid);
	int m=5190,i;
	for(i=0;i<no_of_services;i++)
	{
		key_t key1 = ftok(".",m);
		int shmid1=shmget(key1,1024,IPC_CREAT|0666);
		s[i]=(struct service*)shmat(shmid1,NULL,0);
		m=m+10;
	}
	signal(SIGUSR1,handler);
	printf("staionmaster started:\n");
	services_portno[0]=9000;
	services_portno[1]=9001;
	services_portno[2]=9002;
	//poll fifo and socket files
	struct pollfd fds[3];
	int sfdd;
	for(i=0;i<3;i++)   // creating sockets for each service
	{
		int sfd=socket(family,typ,0);
		sfdd=sfd;
		fds[i].fd=sfd;
		fds[i].events=POLLIN;
		fds[i].revents=0;
		struct sockaddr_in serveraddr;
		serveraddr.sin_family=family;
		serveraddr.sin_port=htons(services_portno[i]);
		
		if(inet_pton(family,"127.0.0.1",&serveraddr.sin_addr) ==0)
		{
			printf("error in address binding\n");
		}
		bind(sfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
		listen(sfd,10);
		// struct sockaddr_in serv;
		// int le;
		// getsockname(sfd,(struct sockaddr*)&serv,&le);
		// char address[100];
		// inet_ntop(family,(struct sockaddr*)&serv,address,sizeof(address));
		// printf("%s %d\n",address,serv.sin_port);
	}

	rfilename[0]="rdfamousfifo1.txt";
	wfilename[0]="wrfamousfifo1.txt";
	rfilename[1]="rdfamousfifo2.txt";
	wfilename[1]="wrfamousfifo2.txt";
	rfilename[2]="rdfamousfifo3.txt";
	wfilename[2]="wrfamousfifo3.txt";
	for(i=0;i<3;i++)
	{
		s[i]->sid=i;
		s[i]->is_alloted=0;
		s[0]->size=i+1;
		cnt++;
	}
	while(1)
	{
		int ret=poll(fds,3,timeout);
		if(ret>0)
		{
			int i;
			for(i=0;i<3;i++)
			{
				if(fds[i].revents & POLLIN)
				{
					struct sockaddr_in peeraddr;
					unsigned int len;
					int nsfd=accept(fds[i].fd,(struct sockaddr*)&peeraddr,&len);

					struct sockaddr_in peer;
					unsigned int l;
					getsockname(nsfd,(struct sockaddr*)&peer,&l);
					char address[100];
					inet_ntop(family,(struct sockaddr*)&peer,address,sizeof(address));
					printf("%s %d\n",address,peer.sin_port);
					struct sockaddr_in peers;
					getpeername(nsfd,(struct sockaddr*)&peers,&l);
					inet_ntop(family,(struct sockaddr*)&peers,address,sizeof(address));
					printf("%s %d\n",address,peers.sin_port);
					
					assert(nsfd>=0);
					printf("train no %d has arrived..\n",i+1);
					int platform_id=is_available();
					if(platform_id!=-1)
					{
						printf("platform no %d has been alloted to train no:%d\n",platform_id+1,i+1);
						write(nsfd,"1",sizeof("1"));
						int j;
						for(j=0;j<3;j++)
						{
							int wfd=open(wfilename[j],0666);
							//printf("%d\n",shared->platformid[j]);
							kill(shared->platformid[j],SIGUSR2);
							//printf("%d\n",wfd);
							if(j==platform_id)
							{
								char str[100];
								sprintf(str,"%s %d","print the train coaches no on platform no:",platform_id+1);
								write(wfd,str,sizeof(str));
							}
							else
							{
								char str[100];
								sprintf(str,"%s %d %s %d","train no ",i+1," has been arrived on platform no:",platform_id+1);
								write(wfd,str,sizeof(str));
							}

						}
					}
					else
					{
						printf("No platform is not avaiable for train no:%d\n",i+1);
						write(nsfd,"0",sizeof("0"));
					}
				}
			}
		}
	}
	return 0;
}