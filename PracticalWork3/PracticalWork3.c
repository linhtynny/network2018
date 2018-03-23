#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h> 

int main(){

int sockfd, clen, clientfd;
struct sockaddr_in saddr, caddr;
unsigned short port = 8784;
char buffer[100] = "testing ";
int cont = 1;

if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	printf("Error creating socket \n");
}
else{
	printf("Socket!\n");
}

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


if ((clientfd = accept(sockfd, (struct sockaddr *)&caddr, (socklen_t *)&clen)) < 0)
{
	printf("Error accepting connection \n ");
}
else{
	printf("Accept!\n");
	while(cont == 1){
		
		recv(clientfd, buffer, sizeof(buffer), 0);
		printf("%s\n", buffer);
		printf("%s\n", "Server:");
		scanf("%s", buffer);
		send(clientfd, buffer, sizeof(buffer), 0);
		// printf("%s\n", "Stop? Type 0 to stop or press enter. " );
		// scanf("%d", &cont);

	}
	close(clientfd);
}
return 0;
}