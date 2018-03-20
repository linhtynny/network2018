#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>

int main(){
	
	char str[80];
	// ask the domain to lookup for ketboard
	printf("%s\n", "Enter the domain name:");
	scanf("%s", str);

	//gethostbyname(str)
	struct hostent *h_name = gethostbyname(str);


	//print the output
	printf("%s\n", "IP address:");
	struct in_addr **ip ;
	ip = (struct in_addr **)h_name->h_addr_list;

	for (int i=0;ip[i] != NULL; i++){
		printf("%s\n", inet_ntoa(*ip[i]));
	}
}