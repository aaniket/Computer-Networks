#include <stdio.h>
#include <assert.h>
#include "square.h"
#include <rpc/rpc.h>
#include <sys/types.h>
int main(int argc, char  **argv)
{
	CLIENT *cl;
	square_in in;
	square_out *out;

	assert(argc==3);

	cl=clnt_create(argv[1],SQUARE_PROG,SQUARE_VERS,"tcp");

	in.arg1=atol(argv[2]);
	if((out==squareproc_1(&in,cl))==NULL)
		printf("Error :");
	printf("result: $ld\n",out->res);

	return 0;
}