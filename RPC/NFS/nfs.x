struct nfs_in{
	char command[1024];
	int flag;
	int size;
	char buffer[1024];
	char filename[2014];
};

struct nfs_out{
	char res[1024];
	int size;
	 
};

program NFS_PROG{
	version NFS_VERS{
		nfs_out NFSPROC(nfs_in) =1;
	}=1;
}= 0x31230001;
