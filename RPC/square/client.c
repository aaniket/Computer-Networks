#include <stdio.h>
#include <assert.h>
#include "square.h"
#include<rpc/rpc.h>

//--> returns CLIENT HANDLE 
//-->CLIENT *client_create(const char *host,unsigned long prognum,unsigned long versnum,const char *protocol);


int main(int argc, char const *argv[])
{
	CLIENT *cl;
	square_in in;
	square_out *out;

	assert(argc==3);

	cl=client_create(argv[1],SQUARE_PROG,SQUARE_VERS,"tcp");

	in.arg1=atol(argv[2]);
	if((out==squareproc_1(&in,cl))==NULL)
		printf("Error :%s",clnt_sperror(cl,argv[1]));
	printf("result: $ld\n",out->res1);

	return 0;
}