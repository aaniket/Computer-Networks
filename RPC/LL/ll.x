struct node{
	
	int val;
	node * next;
};


struct square_out
{
	node * head;
};

program SQUARE_PROG
{
	version SQUARE_VERS1
	{
		square_out SQUAREPROC(node *) =1;

		square_out fuck(node) = 2;
	}=1;

	version SQUARE_VERS2
	{
		square_out fun1(node) = 1;
	}=2;
}= 0x31230000;





