#include<stdio.h>
int main(){
	FILE *fd=fopen("new","r");
	printf("%d\n",fileno(fd));
	return 0;
}