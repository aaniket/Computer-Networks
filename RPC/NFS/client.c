#include "nfs.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <rpc/rpc.h>
#include <sys/types.h>
#include <string.h>
#include <error.h>

int main(int argc,char* argv[])
{
	
	while(1){
		nfs_in in;
		nfs_out *outp;
		//memset(outp->res,0,1023);
		//printf("in loop\n");
		printf("aniket$");
		gets(in.command);
		if(in.command[0]=='r' && in.command[3]=='d'){
			in.flag=1;
			scanf("%s",&in.filename);
			//scanf("%d",&in.start);
			scanf("%d",&in.size);
		}
		else if(in.command[0]=='w' && in.command[4]=='e'){

			in.flag=2;
			scanf("%s",&in.filename);
			//scanf("%d",&in.start);
			scanf("%d",&in.size);	
			char s;
			scanf("%c",&s);
			gets(in.buffer);
			//puts(in.buffer);
			printf("%s\n",in.buffer );
		}
		else{
			in.flag=0;
			printf("\n%s\n",in.command);
		}
		CLIENT *cl;
		cl = clnt_create(argv[1], NFS_PROG, NFS_VERS, "tcp");
		if ( (outp = (nfs_out*)nfsproc_1(&in, cl)) == NULL)
			printf("error\n");
		printf("%s\n",outp->res);
		outp->res[0]='\0';
		//exit (0) ;
	}
}