#include <sys/socket.h>                                 // socket(), bind(), ...
#include <netinet/in.h>                                 // struct sockaddr_in
#include <strings.h>                                    // memset(), ...
#include <unistd.h>                                     // write(), close(), ...
#include <stdio.h>                                      // printf(), sprintf()
#include <arpa/inet.h>                                  // inet_ntoa(), ...
#include <string.h>                                     // strlen()
#include <sys/wait.h>                                   // waitpid()
#include <signal.h>                                     // signal()
#include <stdlib.h>                                     // exi
#define PORTNUMBER 6776                                 // check "/etc/services"

int main (void)
{
	int listenfd, connfd ;
	struct sockaddr_in  servaddr, cliaddr;
	pid_t ForkPID ;
	socklen_t len ;
	int optval = 1;
	int ret = 0;
	char *message, recvBuff[1024];

	// socket() open a file descriptor
	listenfd = socket (AF_INET, SOCK_STREAM, 0) ;
	if (listenfd == -1)
	{
		perror ("socket error");;
		exit(EXIT_FAILURE);
	}
	// setsockopt() free previously used sockets()
	ret = setsockopt (listenfd, SOL_SOCKET, SO_REUSEADDR,
			  &optval, sizeof (optval));
	if (ret == -1)
	{
		perror ("setsockopt error");
		exit(EXIT_FAILURE);
	}
	// fill in the structure required to handle bind()
	memset (&servaddr, 0, sizeof (servaddr));

	servaddr.sin_family = AF_INET ;  // we use IPv4
	// htonl() and htons() convert values between host and
	//   network byte order
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (PORTNUMBER) ;

	// bind()
	ret = bind (listenfd, (struct sockaddr *) &servaddr,
		    sizeof (servaddr));
	if (ret == -1)
	{
		perror ("bind error");
		exit(EXIT_FAILURE);
	}

	// listen()
	ret = listen (listenfd, 5);
	if (ret == -1)
	{
		perror ("listen error");
		exit(EXIT_FAILURE);
	}

	int recv_size=0;
	len = sizeof (cliaddr);
	while ((connfd=accept(listenfd,(struct sockadd *) &cliaddr, &len))>=0) {
		// let the child handle the connection
		ForkPID = fork() ;
		if (ForkPID == 0) {
			// child process
			close (listenfd) ; // close listen port

			//dup2 (connfd, 0);         // redirect STDIN
			//dup2 (connfd, 1);         // redirect STDOUT
        if((recv_size = recv(connfd, recvBuff, 1023 , 0)) < 0){
            puts("recv failed");
        }else{
            puts("Reply received\n");
            recvBuff[recv_size]='\0';
            printf ("String: %s\n",recvBuff);
            message="Login successful\n\0";
            send(connfd, message, strlen(message) , 0);
        }
        }
	 else {
		// parent process
		close (connfd) ;
		}
	}
}

