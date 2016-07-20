#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rpc/rpc.h>
#include <sys/types.h>
#include <string.h>
#include <error.h>

int main(int argc,char* argv[])
{
	CLIENT *cl;
	array_in in;
	array_out *outp;
	printf("%d\n",argc);
	printf("client\n");
	in.size=argc-2;
	int i=0;
	for(i=0;i<argc-2;i++){
		in.value[i]=atoi(argv[i+2]);
		printf("%d ",in.value[i]);
	}
	cl = clnt_create(argv[1], ARRAY_PROG, ARRAY_VERS, "tcp");

	
	if ( (outp = (array_out*)arrayproc_1(&in, cl)) == NULL)
	printf("error\n");
	printf("client\n");
	printf("resu1t: %ld\n", outp->res);

	cl = clnt_create(argv[1], ARRAY_PROG, ARRAY_VERS1, "tcp");
	if ( (outp = (array_out*)arrayproc_2(&in, cl)) == NULL)
	printf("error\n");
	printf("client\n");
	printf("resu1t: %ld\n", outp->res);


	exit (0) ;
}