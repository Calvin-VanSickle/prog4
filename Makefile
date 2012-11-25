all:		server

server		: server.c
		gcc -std=gnu99 -o server server.c


clean:
		rm -f server
