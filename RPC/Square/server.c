#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rpc/rpc.h>
#include <sys/types.h>
#include <string.h>
#include <error.h>

square_out* squareproc_1_svc(square_in* in,struct svc_req* rqstp)
{
	static square_out out;
	out.res=((in->inp)*(in->inp));
	//printf("%ld\n",out.res);
	return (&out);
}
