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
#define max_client 4
#define mports 5
int gsfd;
int nsfds[10];
int pids[10];
int score[10];
#define ADDRESS      "unix"
#define ADDRESS1     "unix1"
#define ADDRESS2     "unix2"
#define ADDRESS3     "unix3"


struct SHM{
	int flag;
}shm;

struct SHM *shared;
struct SHM *temp;

key_t key=101014;

void init(){
	int shmid=shmget(key, sizeof shm,IPC_CREAT|0666);
	shared=shmat(shmid, NULL, 0);
	
}

int connect_unix(int val){
  printf("connecting UDS %d \n",val);
	int usfd;
	struct sockaddr_un userv_addr;
  	int userv_len,ucli_len;

  	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

  	if(usfd==-1)
  	perror("\nsocket  ");
	printf("Connection succesful1\n");


  	bzero(&userv_addr,sizeof(userv_addr));
  	userv_addr.sun_family = AF_UNIX;
  	char * pth=getenv("PWD");
  	if(val==0)
  		strcpy(userv_addr.sun_path, ADDRESS);
  	if(val==1)
  		strcpy(userv_addr.sun_path, ADDRESS1);
  	if(val==2)
  		strcpy(userv_addr.sun_path, ADDRESS2);
  	if(val==3)
  		strcpy(userv_addr.sun_path, ADDRESS3);

	userv_len = sizeof(userv_addr);
printf("co1\n");

	if(connect(usfd,(struct sockaddr *)&userv_addr,userv_len)==-1)
	perror("\n connect ");

	else printf("\nconnect succesful");

	return usfd;

}

int send_fd(int socket, int fd_to_send)
 {
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket, &socket_message, 0);

 }
 


passfd(int fd){

	
	int val=-1;
	while(val==-1){
		int i;
		temp=shared;
		for(i=0;i<4;i++){
			if(temp->flag==0){
				temp->flag=1;
				val=i;
				break;
			}
			temp++;
		}
	}

	int usfd=connect_unix(val);
 	//printf("hate you unix! %d\n",fd);
 	send_fd(usfd,fd);
}



void scores(){
	int i;
	for(i=0;i<10;i++){
		score[i]=0;
	}
}

void* clnt(void *arg){
	int i=0;
	int id=(int *)arg;
	printf("id : %d\n", id);
	int sc=0;
	int x=2;
	while(x>0){	
		char buffer[1024],buffer1[1024];
		sprintf(buffer,"5+5");
		write(nsfds[id],buffer,sizeof(buffer));
		sleep(rand()%3);
		int n=read(nsfds[id],buffer1,sizeof(buffer1));
		buffer1[n]='/0';
		int value=atoi(buffer1);
		if(value==10){
			score[id]++;
			sc++;
			printf("score1 of client %d is : %d\n",id,sc );
		}
	//	sleep(3);
		x--;
	}
	char val1[10];
	val1[0]='/0';
	sprintf(val1,"1154");
	write(nsfds[id],val1,4);
	printf("%d\n",sc);
	printf("candidate %d succesful1y completed apti , passing to interviewer\n",id);


	//sleep(3);
	passfd(nsfds[id]);
//printf("sorry but your score is not enough !\n");
	//sleep(3);
	//exit(0);
}



int main(int argc,char *argv[]){
	//Shared memory initialisation
	//sleep(5);
	init();
	printf("started\n");
	pthread_t cl;
	//intialise scores
	scores();

	int sfd,nsfd,port,clength,cnt=0;
	int port1=atoi(argv[1]);
	struct sockaddr_in s_addr,c_addr;
	char buffer[1024];
	sfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sfd>=0);
	
	s_addr.sin_family=AF_INET;
 	//s_addr.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY binds to all availible interfaces
	s_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	s_addr.sin_port=htons(port1);

	bind(sfd,(struct sockaddr_in *)&s_addr ,sizeof(s_addr));

	listen(sfd,max_client);
	printf("Not connected to any client ...\n");
	while(1){
			
			nsfd=accept(sfd,(struct sockaddr *) &c_addr,&clength);
			nsfds[cnt]=nsfd;
			char buffer[1024];
			read(nsfd,buffer,sizeof(buffer));

			pids[cnt]=atoi(buffer);
			printf("pid:%d\n",pids[cnt]);

			pthread_create(&cl,NULL,clnt,(void *)cnt);
			sleep(2);
			cnt++;

	}

	return 0;
}