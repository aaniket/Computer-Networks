/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "ll.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

square_out *
squareproc_1(node *argp, CLIENT *clnt)
{
	static square_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, SQUAREPROC,
		(xdrproc_t) xdr_node, (caddr_t) argp,
		(xdrproc_t) xdr_square_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

square_out *
fuck_1(node *argp, CLIENT *clnt)
{
	static square_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, fuck,
		(xdrproc_t) xdr_node, (caddr_t) argp,
		(xdrproc_t) xdr_square_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

square_out *
fun1_2(node *argp, CLIENT *clnt)
{
	static square_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, fun1,
		(xdrproc_t) xdr_node, (caddr_t) argp,
		(xdrproc_t) xdr_square_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
