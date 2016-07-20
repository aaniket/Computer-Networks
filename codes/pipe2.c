#include <stdio.h>
#include <stdlib.h>

int main(){
	char buffer[1024];
	printf("pipe2 started\n");
	sprintf(buffer,"hello ");
	while(1){
		write(1,buffer,7);
		sleep(rand()%4);
	}
	return 0;
}