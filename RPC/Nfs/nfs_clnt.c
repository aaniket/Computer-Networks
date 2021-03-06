/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "nfs.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

nfs_out *
nfsproc_1(nfs_in *argp, CLIENT *clnt)
{
	static nfs_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, NFSPROC,
		(xdrproc_t) xdr_nfs_in, (caddr_t) argp,
		(xdrproc_t) xdr_nfs_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
