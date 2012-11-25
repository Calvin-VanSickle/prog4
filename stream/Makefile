all	: server mserver client

server:	server.c
	gcc -std=c99 server.c -o server

mserver:	mserver.c
	gcc mserver.c -o mserver

client:	client.c
	gcc client.c -o client

clean	:
	rm -f server mserver client
