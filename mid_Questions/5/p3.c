#include <stdio.h>
#include <unistd.h>

int main()
{
	while(1)
	{
		write(1,"process3data\n",sizeof("process3data\n"));
		sleep(1);
	}
	return 0;
}