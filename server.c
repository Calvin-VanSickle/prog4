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

typedef struct spawn_t{
	int pipe[2];
	int guesses;
	char name[100];
} spawn;


int main(void){
	char input[11] = {0};
	spawn childs[101];
	// to keep track of where our unused pipe spots are
	int childi = 0;
	memset(childs, 0, sizeof(spawn) * 101);
    // set up socket (in process)
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	int i;
	
    // create destination
    struct sockaddr_in s_in;
    memset((char *) &s_in, sizeof(s_in),0); 
    s_in.sin_family = (short)AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons((unsigned short)4545);

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
		//int select(int nfds, fd_set *readfds, fd_set *writefds,
		//					fd_set *exceptfds, struct timeval *timeout);
		int num_active = select(socket_fd + 1, &read_set, NULL, NULL, NULL);
		if( num_active > 1){
			printf("select behaving wierd\n"); exit(1);
		}
		if(FD_ISSET(0,&read_set)){
			fgets(input, 10, stdin);
			if(strcmp("show",input) == 0){
				
			}else if(strcmp("exit",input) == 0){
				exit(0);
			}
		}else if(FD_ISSET(socket_fd,&read_set)){
			struct sockaddr_in s_out;
			length = sizeof(s_out);
		
			// accept a connection
			int conn_fd = accept(socket_fd, (struct sockaddr*)&s_out, &length);
			if (conn_fd < 0) { perror("accept"); exit(1); }
			
			if(pipe(childs[childi].pipe)){perror("pipin"); exit(1);}
			
			int randomnum = (random() % 10) + 1;
			int guess = 0;
			
			printf("randy: %i\n", randomnum);
			// trying with out forking right now
			/*int cpid = fork();
			if(cpid){
				//child
				exit(1);
			}else{
				
			}*/
			read(conn_fd,childs[0].name, 80);
			printf("%s\n",childs[0].name);
			read(conn_fd,&guess, 4);
			printf("guess: %i\n",ntohl(guess));
			read(conn_fd,&guess, 4);
			printf("guess: %i\n",ntohl(guess));
			read(conn_fd,&guess, 4);
			printf("guess: %i\n",ntohl(guess));
			read(conn_fd,&guess, 4);
			printf("guess: %i\n",ntohl(guess));
			read(conn_fd,&guess, 4);
			printf("guess: %i\n",ntohl(guess));
			
			exit(0);
			
			FD_SET(socket_fd,&read_set);
		}else{
			//must be one of our children dieing!
			
			// wait for it
		}
		// reset the fd set
		FD_ZERO(&read_set);
		FD_SET(socket_fd,&read_set);
		FD_SET(0,&read_set);
		// look through our childs. only look at the ones which we could have
		// used, and only set the ones which have't returned guesses and shut
		// down yet
		for(i = 0; i < childi; i++){
			if(childs[i].guesses == 0){
				FD_SET(childs[i].pipe[1],&read_set);
			}
		}
	}
}
