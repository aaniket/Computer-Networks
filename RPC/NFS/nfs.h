/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _NFS_H_RPCGEN
#define _NFS_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct nfs_in {
	char command[1024];
	int flag;
	int size;
	char buffer[1024];
	char filename[2014];
};
typedef struct nfs_in nfs_in;

struct nfs_out {
	char res[1024];
	int size;
};
typedef struct nfs_out nfs_out;

#define NFS_PROG 0x31230001
#define NFS_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define NFSPROC 1
extern  nfs_out * nfsproc_1(nfs_in *, CLIENT *);
extern  nfs_out * nfsproc_1_svc(nfs_in *, struct svc_req *);
extern int nfs_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define NFSPROC 1
extern  nfs_out * nfsproc_1();
extern  nfs_out * nfsproc_1_svc();
extern int nfs_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_nfs_in (XDR *, nfs_in*);
extern  bool_t xdr_nfs_out (XDR *, nfs_out*);

#else /* K&R C */
extern bool_t xdr_nfs_in ();
extern bool_t xdr_nfs_out ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_NFS_H_RPCGEN */