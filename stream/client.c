#include <errno.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>

#define h_addr h_addr_list[0]

/**
 * Send a stream to server.
 */
int main(int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: client hostname portnumber\n");
        exit(1);
    }

    // get port number
    unsigned short port = atoi(argv[2]);

    // set up socket to networked machine
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) { perror("socket"); exit(1); }

    // create socket address (of server)
    struct sockaddr_in dest;
    memset((char *) &dest, sizeof(dest),0);
    struct hostent* hostptr = gethostbyname(argv[1]);
    if (!hostptr) { herror("lookup error"); exit(1);}
    dest.sin_family = (short) AF_INET;
    memcpy((char *)&dest.sin_addr, (char *)hostptr->h_addr, hostptr->h_length);
    dest.sin_port = htons(port);

    // connect to the server
    int cret = connect(socket_fd,(struct sockaddr*)&dest, sizeof(dest));
    if (cret < 0) { perror("connect"); exit(1);}

    // set up message and sizes
    char msg[] = "a stitch in time saves nine";
    int left = sizeof(msg);
    int start = 0;

    // keep writing until the whole message has been sent.
    while (left > 0) {
	int num = write(socket_fd,msg+start,left);
	printf("%d bytes sent\n", num);
	if (num < 0) { perror("write"); exit(1); }
	else {
	    left -= num;
	    start += num;
	}
    }

} // end of main
