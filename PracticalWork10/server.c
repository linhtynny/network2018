#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <pthread.h>
#include <poll.h>


#define MAX_CLIENT 100

int clientfds[100]; // list of connected clients, >0 if valid
int sockfd, clen, clientfd;
char buffer[100];

void *input(){
	while(1){
		printf("%s ", "Server: ");
		scanf("%s", buffer);
		for(int i=0; i<MAX_CLIENT; i++) {
			if (clientfds[i] > 0){
				send(clientfds[i], buffer, sizeof(buffer), 0);
			}
		}
	}
}

int main(){

	
	struct sockaddr_in saddr, caddr;
	unsigned short port = 8785;
	

	

	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket \n");
	}
	else{
		printf("Socket!\n");
	}

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));

	int fl = fcntl(sockfd, F_GETFL, 0);
	      fl |= O_NONBLOCK;
	      fcntl(sockfd, F_SETFL, fl);

	memset(&saddr, 0, sizeof(saddr)); 
	saddr.sin_family = AF_INET; 
	saddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	saddr.sin_port = htons(port);



	if ((bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)) { 
		printf("Error binding \n");
	}
	else{
		printf("Bind!\n");
	}

	if (listen(sockfd, 5) < 0) { 
		printf("Error listening \n");
	}
	else{
		printf("Listen!\n");
	}

	pthread_t inputthread;
	pthread_create(&inputthread, NULL,input, NULL);


	clen=sizeof(caddr);
	// clientfd = accept(sockfd, (struct sockaddr *)&caddr, (socklen_t *)&clen);

	
	memset(clientfds, 0, sizeof(clientfds)); // clear the list


	while(1){
		fd_set set; // declaration of the set
		FD_ZERO(&set); //clear the set
		FD_SET(sockfd, &set);
		int maxfd = sockfd;
		for (int i = 0; i < 100; i++) { // add connected client sockets to set
			if (clientfds[i] > 0) FD_SET(clientfds[i], &set);
			if (clientfds[i] > maxfd) maxfd = clientfds[i]; 
		}
		// poll and wait, blocked indefinitely
		select(maxfd+1, &set, NULL, NULL, NULL);

		// a «listening» socket?
		if (FD_ISSET(sockfd, &set)) {
		clientfd = accept(sockfd, (struct sockaddr *) &saddr, (socklen_t *)&clen);
		    // make it nonblocking
		    fl = fcntl(clientfd, F_GETFL, 0);
		    fl |= O_NONBLOCK;
		    fcntl(clientfd, F_SETFL, fl);
		    // add it to the clientfds array
			for (int i = 0; i < MAX_CLIENT; i++) { 
				if (clientfds[i] == 0) {
					if (clientfds[i] == 0) {
						clientfds[i] = clientfd;
						break; 
					}
				}
			}
		}

		// is that data from a previously-connect client?
		for (int i = 0; i < MAX_CLIENT; i++) {
			if (clientfds[i] > 0 && FD_ISSET(clientfds[i], &set)) {
				if (read(clientfds[i], buffer, sizeof(buffer)) > 0) {
					printf("client %d says: %s\nserver>", clientfds[i], buffer);
				}
				else {
			            // some error. remove it from the "active" fd array
			            printf("client %d has disconnected.\n", clientfds[i]);
			            clientfds[i] = 0;
			    }
			} 
		}

	}
	
	return 0;
}