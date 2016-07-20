/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "array.h"

bool_t
xdr_array_in (XDR *xdrs, array_in *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->value, sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->size))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_array_out (XDR *xdrs, array_out *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->res))
		 return FALSE;
	return TRUE;
}
