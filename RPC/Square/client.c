#include "square.h"
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
	square_in in;
	square_out *outp;
	printf("%d\n",argc);
	cl = clnt_create(argv[1], SQUARE_PROG, SQUARE_VERS, "tcp");
	printf("client\n");
	in.inp = atol(argv[2]);
	printf("%ld\n",in.inp);
	if ( (outp = (square_out*)squareproc_1(&in, cl)) == NULL)
	printf("error\n");
	printf("client\n");
	printf("resu1t: %ld\n", outp->res);
	exit (0) ;
}