#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>


int main(void){
    // set up socket (in process)
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // create destination
    struct sockaddr_in s_in;
    memset((char *) &s_in, sizeof(s_in),0); 
    s_in.sin_family = (short)AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons((unsigned short)5001);

    // bind socket to a port
    int length = sizeof(s_in);
    bind(socket_fd, (struct sockaddr*)&s_in, length);

    // print port number
    getsockname(socket_fd, (struct sockaddr*)&s_in, &length);
    printf("%d", ntohs(s_in.sin_port));
	fflush(stdout);

    // listen
    int lret = listen(socket_fd, 1);
    if (lret < 0) { perror("listen"); exit(1); }
	
	// wait using select
	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(socket_fd,&read_set);
	FD_SET(0,&read_set);
	while(1){
		//int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
		select(socket_fd + 1, &read_set, NULL, NULL, NULL);
		
		if(FD_ISSET(0,&read_set)){
			// got somthing on 
			printf("select got unplugged by stds in\n");
			exit(1);
		}else if(FD_ISSET(0,&read_set)){
			
			printf("select got unplugged by stds in\n");
			struct sockaddr_in s_out;
			length = sizeof(s_out);
		
			// accept a connection
			int conn_fd = accept(socket_fd, (struct sockaddr*)&s_out, &length);
			if (conn_fd < 0) { perror("accept"); exit(1); }
		
			printf("fork\n");
			//int cpid = fork();
			exit(1);
		}
		
		
	}
}