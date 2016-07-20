#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(){
	int n;
	scanf("%d",&n);
	if(n%2==0)
		printf("even\n");
	else
		printf("odd\n");
	return 0;
}