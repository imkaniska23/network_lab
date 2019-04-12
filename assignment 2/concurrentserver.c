#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#define max 100
#define port 8080
void func(int connfd)
{
	char buff[max];
	int n;
	for(;;)
	{
		bzero(buff,max);
		read(connfd,buff,sizeof(buff));
		printf("Message from client:%s\n",buff);
		bzero(buff,max);
		printf("Enter message: ");
		n=0;
		while((buff[n++]=getchar())!='\n')
			;
		write(connfd,buff,sizeof(buff));
		if(strncmp(buff,"exit",4)==0)
		{
			printf("Server exiting...\n");
			break;
		}
	}
}
int main()
{
	int connfd, sockfd, len;
	struct sockaddr_in servaddr, cli;
	pid_t childpid;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created...\n");
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(port);
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
	{
		printf("Binding failed...\n");
		exit(0);
	}
	else
		printf("Binding successfull...\n");
	if(listen(sockfd,8)==-1)
	{
		printf("Listening failed...\n");
		exit(0);
	}
	else
	{
		for(;;)
		{
			len=sizeof(cli);
			connfd=accept(sockfd,(struct sockaddr*)&cli,&len);
			if((childpid=fork())==0)
			{
				close(sockfd);
				func(connfd);
			}
			close(connfd);
		}
	}
}