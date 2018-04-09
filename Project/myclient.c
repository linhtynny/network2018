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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sleepcond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sleepmutex = PTHREAD_MUTEX_INITIALIZER;

char buffer[100],sendbuffer[100];
int sockfd;
int p[2];

void *input()
{
   while(1){
        printf("Client: ");
        pthread_mutex_lock(&mutex);
        // printf("Client: ");
        scanf("%s", buffer);
        dup2(p[1],1);
        // close(p[1]);
        // close(p[0]);
        // for(int i=0; i<MAX_CLIENT; i++) {
        //  if (clientfds[i] > 0){
        //      send(clientfds[i], buffer, sizeof(buffer), 0);
        //  }
        // }
        pthread_mutex_unlock(&mutex);
    }
}

void *network(void *sockfd)
{
    int tempfd = *((int *)sockfd);
    while (1){
        pthread_mutex_lock(&mutex);
        dup2(p[0],0);
        // close(p[0]);
        // close(p[1]);
        send(tempfd, buffer, sizeof(buffer), 0);
        memset(&buffer, 0, sizeof(buffer));
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char **argv)
{
struct sockaddr_in saddr;
struct hostent *h;
struct in_addr **ip ;
unsigned short port = 8784;
char host[100];

    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error creating socket\n");
}
else{
    printf("Socket! \n");
}
    
printf("Please enter hostname: ");
scanf("%s", host);

if ((h=gethostbyname(host)) == NULL) { 
    printf("Unknown host \n");
    // printf("%s\n", "Enter hostname:");
    // scanf("%s", host);
}
else{
    ip = (struct in_addr **)h->h_addr_list;
    printf("IP address: \n");
    for (int i=0;ip[i] != NULL; i++){
        printf("%s\n", inet_ntoa(*ip[i]));
    }
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *) &saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    saddr.sin_port = htons(port);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("Cannot connect, error is %d\n", errno);
    }
    else{
        int fl = fcntl(sockfd, F_GETFL, 0);
              fl |= O_NONBLOCK;
              fcntl(sockfd, F_SETFL, fl);

        printf("Connect!\n");
//  }
// }
// read(sockfd, buffer, sizeof(buffer));
           
            pthread_t th_input, th_network;
            // printf("Client: ");
            pipe(p);
            pthread_create(&th_input, NULL,input, NULL);
            close(p[1]);
            close(p[0]);
            pthread_create(&th_network, NULL, network, &sockfd);
            close(p[1]);
            close(p[0]);
            while (1)
            {
                // pipe(p);
                while (recv(sockfd, sendbuffer, sizeof(sendbuffer), 0) > 0)
                {
                    printf("%s\n", sendbuffer);
                }
                memset(&sendbuffer, 0, sizeof(sendbuffer));
            }
            pthread_join(th_input, NULL);
            pthread_join(th_network, NULL);
            
        }
    }
    return 0;
}