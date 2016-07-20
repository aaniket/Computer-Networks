#include <stdio.h>

int main()
{
	//while(1)
	//{
		char buf[30];
		read(0,buf,sizeof(buf));
		printf("the output is:%s\n",buf);
	//}
	return 0;
}