#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#define max 80
#define port 8080
void func(int connfd)
{
	char buff[max];
	int n;
	for(;;)
	{
		bzero(buff,max);
		read(connfd,buff,sizeof(buff));
		printf("From client to server: %s \n",buff);
		bzero(buff,max);
		n=0;
		printf("Enter message: ");
		while((buff[n++]=getchar())!='\n')
			;
		write(connfd,buff,sizeof(buff));
		if(strncmp("exit",buff,4)==0)
		{
			printf("Server exit.\n");
			break;
		}
	}
}
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	sockfd=socket(AF_INET, SOCK_STREAM,0);
	if(sockfd==-1)
		printf("Socket creation failed... \n");
	else
		printf("Socket successfully created... \n");
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(port);
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))!=0)
	{
		printf("Socket binding failed... \n");
		exit(0);
	}
	else
		printf("Socket successfully created... \n");
	if(listen(sockfd,5)!=0)
	{
		printf("Listen failed... \n");
		exit(0);
	}
	else
	{
		printf("Server successfully listening... \n");
		len=sizeof(cli);
	}
	connfd=accept(sockfd, (struct sockaddr*)&cli, &len);
	if(connfd<0)
	{
		printf("Server accept failed... \n");
		exit(0);
	}
	else
		printf("Server successfully accepted... \n");
	func(connfd);
	close(sockfd);
	close(connfd);
}