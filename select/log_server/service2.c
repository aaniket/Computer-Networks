// service no 2
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//testing whether a number is prime or not

int main(int argc,char* argv[])
{
	int n;
	scanf("%d",&n);
	int i,fl=0;
	for(i=2;i<n;i++)
	{
		if(n%i==0)
		{
			fl=1;
			break;
		}
	}
	if(fl==0)
	{
		printf("The given number is prime.\n");
	}
	else
	{
		printf("The given number is not prime.\n");
	}
	return 0;
}