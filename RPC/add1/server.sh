cc -c server.c -o server.o
cc -c array_svc.c -o array_svc.o
cc -o server server.o array_svc.o array_xdr.o