rpcgen -C array.x
cc -c client.c -o client.o -w
cc -c array_clnt.c -o array_clnt.o
cc -c array_xdr.c -o array_xdr.o
cc -o client client.o array_clnt. array_xdr.o 
