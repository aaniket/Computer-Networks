#include <stdio.h>

int main(void) {
	FILE *val;
	extern FILE *popen();
	char buff[512];

	if(!(val = popen("ls", "r"))){
		printf("error\n");
	}

	while(fgets(buff, sizeof(buff), val)!=NULL){
		printf("%s", buff);
	}
	pclose(val);

}