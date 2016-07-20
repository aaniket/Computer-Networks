#include<stdio.h>
#include<stdlib.h>
int main(){
	//char buf[20];
	FILE *rfd,*wfd,*fd,*fd1;
	fd=fopen("input.txt","r");
	fd1=fopen("output.txt","r");;
	rfd=popen("/home/aniket/Desktop/CN/popen/aniket","w");
	dup2(fd,rfd);
	wfd=popen("/home/aniket/Desktop/CN/popen/aniket","r");
	char* buf;
	read(wfd,buf,6);
	//dup2(fd1,wfd);
	write(fd1,buf,6);
	return 0;

}