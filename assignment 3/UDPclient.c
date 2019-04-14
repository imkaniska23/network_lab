#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define max 100
#define port 8080
int main()
{
	int sockfd;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created...\n");
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servaddr.sin_port=htons(port);
	char buff[max];
	int n;
	int len=sizeof(servaddr);
	for(;;)
	{
		printf("Enter message for server:");
		bzero(buff,max);
		n=0;
		while((buff[n++]=getchar())!='\n')
			;
		sendto(sockfd,buff,sizeof(buff),0,(const struct sockaddr*)&servaddr,sizeof(servaddr));
		bzero(buff,max);
		printf("Message from server:");
		recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&servaddr,&len);
		printf("%s\n");
		if(strncmp(buff,"exit",4)==0)
		{
			printf("Client exiting...\n");
			break;
		}
	}
	close(sockfd);
	return 0;
}