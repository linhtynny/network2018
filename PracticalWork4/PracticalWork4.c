#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>

int main(){

struct sockaddr_in saddr;
struct hostent *h;
int sockfd;
unsigned short port = 8784;
char str[80], buffer[100];
int cont = 1;

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

memset(&saddr, 0, sizeof(saddr));
saddr.sin_family = AF_INET;

memcpy((char *) &saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
saddr.sin_port = htons(port);

if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("Cannot connect\n");
}
else{
	printf("Connect!\n");// read(sockfd, buffer, sizeof(buffer));
	while(cont == 1){
		
		printf("%s\n", "Client:");
		scanf("%s", buffer);
		send(sockfd, buffer, sizeof(buffer), 0);
		recv(sockfd, buffer, sizeof(buffer), 0);
		printf("%s\n", buffer);
		// printf("%s\n", "Stop? Type 0 to stop or press enter. " );
		// scanf("%d", cont);
	}
	close(sockfd);
}
}