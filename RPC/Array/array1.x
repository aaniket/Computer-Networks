struct array_in
{
	int *value;
	int size;
};

struct array_out
{
	int res;
	 
};

program ARRAY_PROG
{
	version ARRAY_VERS
	{
		ARRAY_out ARRAYPROC(square_in) =1;
	}=1;
}= 0x31230000;
