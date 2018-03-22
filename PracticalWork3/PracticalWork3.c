#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <string.h>
#include <unistd.h> 

int main(){

int sockfd, clen, clientfd;
struct sockaddr_in saddr, caddr;
unsigned short port = 8784;


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

while(1){
if ((clientfd = accept(sockfd, (struct sockaddr *)&caddr, (socklen_t *)&clen)) < 0)
{
	printf("Error accepting connection \n ");
}
else{
	printf("Accept!\n");
}
}
return 0;
}