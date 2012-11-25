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

/**
 * Set up server to receive a stream.
 */
int main() {

    // set up socket (in process)
    int socket_fd = socket (AF_INET, SOCK_STREAM, 0);

    // create destination
    struct sockaddr_in s_in;
    memset((char *) &s_in, sizeof(s_in),0); 
    s_in.sin_family = (short)AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons(0);	// let system assign a port

    // bind socket to a port
    int length = sizeof(s_in);
    bind(socket_fd, (struct sockaddr*)&s_in, length);

    // print port number
    getsockname(socket_fd, (struct sockaddr*)&s_in, &length);
    printf("port number %d assigned\n", ntohs(s_in.sin_port));

    // listen
    int lret = listen(socket_fd, 1);
    if (lret < 0) { perror("listen"); exit(1); }

    // accept
    struct sockaddr_in s_out;
    length = sizeof(s_out);
    int conn_fd = accept(socket_fd, (struct sockaddr*)&s_out, &length);
    if (conn_fd < 0) { perror("accept"); exit(1); }

    // read 
    printf("Connection from: %s\n", inet_ntoa(s_out.sin_addr));
    printf("Data received: ");
    char ch;
    while (read(conn_fd,&ch,1) == 1)
	putchar(ch);
    putchar('\n');

} // end of main
