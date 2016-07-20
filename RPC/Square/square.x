struct node{
	
	int val;
	node * next;
}

struct square_in
{
	node * head;
};

struct square_out
{
	long res;
};

program SQUARE_PROG
{
	version SQUARE_VERS
	{
		square_out SQUAREPROC(square_in) =1;
	}=1;
}= 0x31230000;





