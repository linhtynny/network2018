#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>

int main(){

int sockfd, clen, clientfd;
struct sockaddr_in saddr, caddr;
unsigned short port = 8785;
char buffer[100] = "testing ";

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

clen=sizeof(caddr);

while (1){
	if ((clientfd = accept(sockfd, (struct sockaddr *)&caddr, (socklen_t *)&clen)) > 0)
	{
		printf("Accept!\n");

		int fl = fcntl(clientfd, F_GETFL, 0);
		      fl |= O_NONBLOCK;
		      fcntl(clientfd, F_SETFL, fl);


		while(1){
			while(recv(clientfd, buffer, sizeof(buffer), 0) >0){
				printf("%s ", "Client: ");
				printf("%s\n", buffer);
			}
			printf("%s ", "Server: ");
			memset(buffer,0,sizeof(buffer));

			// write msg to buffer
			// int i = 0;
			// while (1) {
			//     scanf("%c", &buffer[i]);
			//     if (buffer[i] == '\n') {
			//       break;
			//     }
			//     else {
			//       i++;
			//     }
			//   }
			scanf("%s", buffer);
			send(clientfd, buffer, sizeof(buffer), 0);
		}
		close(clientfd);
		// }
	}
}
return 0;
}