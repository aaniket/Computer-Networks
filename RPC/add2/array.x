struct array_in{
	int value[100];
	int size;
};

struct array_out{
	int res;
	 
};

program ARRAY_PROG{
	version ARRAY_VERS{
		array_out ARRAYPROC(array_in) =1;
	}=1;

	version ARRAY_VERS1{
	array_out ARRAYPROC(array_in) =1;
	}=2;
}= 0x31230001;
