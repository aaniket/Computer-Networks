#include "nfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rpc/rpc.h>
#include <sys/types.h>
#include <string.h>
#include <error.h>

int main(int argc,char* argv[])
{
	nfs_in in;
	nfs_out *outp;
	printf("Service 	Service 	arguments\n1.read-filename,starting index,size\n2.write-filename,size,data\nenter prefered service details\n");
	scanf("%d",&in.command);
	if(in.command==1){
		scanf("%s",&in.filename);
		scanf("%d",&in.start);
		scanf("%d",&in.size);

	}
	else if(in.command==2){
		scanf("%s",&in.filename);
	//	scanf("%d",&in.start);
		scanf("%d",&in.size);	
		char s;
		scanf("%c",&s);
		gets(in.buffer);
		//puts(in.buffer);
		printf("%s\n",in.buffer );
	}

	CLIENT *cl;
	
	cl = clnt_create(argv[1], NFS_PROG, NFS_VERS, "tcp");


	
	if ( (outp = (nfs_out*)nfsproc_1(&in, cl)) == NULL)
	printf("error\n");
	//printf("client\n");
	if(in.command==1){
		printf("resu1t: %s\n", outp->res);
	}
	else{
		printf("write succesful!\n");
	}

	exit (0) ;
}