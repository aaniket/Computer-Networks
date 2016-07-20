#include "nfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rpc/rpc.h>
#include <sys/types.h>
#include <string.h>
#include <error.h>

nfs_out* nfsproc_1_svc(nfs_in* in,struct svc_req* rqstp)
{
	static nfs_out out;
	if(in->command==1){
		FILE * fp=fopen(in->filename,"r");
		char buffer[in->size];
		char *waste=(char*)malloc(sizeof(char)*1024);
		char *valued=(char*)malloc(sizeof(char)*1024);

		fgets(waste,in->start-1,fp);
		fgets(valued,in->size,fp);
		fclose(fp);
		strncpy(out.res,valued,in->size);
		out.size=in->size;
	}
	else if(in->command==2){

		FILE * fp=fopen(in->filename,"w");

		char *valued=(char*)malloc(sizeof(char)*1024);
		fgets(valued,in->size,fp);
		strcat(valued,in->buffer);
		fputs(valued,fp);
		out.size=in->size;
		fclose(fp);
	}
	return (&out);
}