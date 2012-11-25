all:		server

server		: server.c
		gcc -std=c99 -o server server.c


clean:
		rm -f server
