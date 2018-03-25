#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <pthread.h>


int main(){

struct sockaddr_in saddr;
struct hostent *h;
int sockfd;
unsigned short port = 8785;
char str[80], buffer[100];

if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error creating socket\n");
}
else{
    printf("Socket! \n");
}
if ((h=gethostbyname("localhost")) == NULL) { 
	printf("Unknown host \n");
	printf("%s\n", "Enter hostname:");
	scanf("%s", str);
	h = gethostbyname(str);
}

setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));

// int fl = fcntl(sockfd, F_GETFL, 0);
//       fl |= O_NONBLOCK;
//       fcntl(sockfd, F_SETFL, fl);


memset(&saddr, 0, sizeof(saddr));
saddr.sin_family = AF_INET;

memcpy((char *) &saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
saddr.sin_port = htons(port);



if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("Cannot connect, error is %d\n", errno);
}
else{
	int fl = fcntl(sockfd, F_GETFL, 0);
	      fl |= O_NONBLOCK;
	      fcntl(sockfd, F_SETFL, fl);

	printf("Connect!\n");// read(sockfd, buffer, sizeof(buffer));
	while(1){

		while(recv(sockfd, buffer, sizeof(buffer), 0)>0){
			printf("%s ", "Server: ");
			printf("%s\n", buffer);
		}
		// free(buffer);
		memset(buffer,0,sizeof(buffer));
		printf("%s ", "Client: ");
		scanf("%s", buffer);

		send(sockfd, buffer, sizeof(buffer), 0);

	}
	close(sockfd);
}
}