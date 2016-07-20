#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rpc/rpc.h>
#include <sys/types.h>
#include <string.h>
#include <error.h>

array_out* arrayproc_1_svc(array_in* in,struct svc_req* rqstp)
{
	static array_out out;
	out.res=0;
	int x=in->size,j=0,i=0;
	printf("%d\n",x);
	// for(i=0;i<x;i++){
	// 	printf("%d ",in->value[i]);
	// }
	for(j=0;j<x;j++){
		out.res=out.res+in->value[j];
		printf("%d\n",(int)(in->value[j]));
	}
	printf("sum %d\n",out.res);
	return (&out);
}

array_out* arrayproc_2_svc(array_in* in,struct svc_req* rqstp)
{
	static array_out out;
	out.res=1;
	int x=in->size,j=0,i=0;
	printf("%d\n",x);
	// for(i=0;i<x;i++){
	// 	printf("%d ",in->value[i]);
	// }
	for(j=0;j<x;j++){
		out.res=out.res*in->value[j];
		printf("%d\n",(int)(in->value[j]));
	}
	printf("multiplication %d\n",out.res);
	return (&out);
}
