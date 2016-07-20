// service no 1
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//finding nth fibnacci number
int f[100005];
int main(int argc,char* argv[])
{
	int n;
	scanf("%d",&n);
	int i,fl=0;
	f[1]=0;
	f[2]=1;
	for(i=3;i<=n;i++)
	{
		f[i]=f[i-1]+f[i-2];
	}
	printf("The %dth fibonacci number is: %d",n, f[n]);
	return 0;
}