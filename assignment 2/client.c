#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#define max 80
#define port 8080
void func(int sockfd)
{
	char buff[max];
	int n;
	for(;;)
	{
		bzero(buff,sizeof(buff));
		printf("Enter string:");
		n=0;
		while((buff[n++]=getchar())!='\n')
			;
		write(sockfd,buff,max);
		bzero(buff,max);
		read(sockfd,buff,max);
		printf("Message from server: %s \n",buff);
		if(strncmp(buff,"exit",4)==0)
		{
			printf("Client exit... \n");
			break;
		}
	}
}
int main()
{
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
		{
			printf("Socket creation failed... \n");
			exit(0);
		}
	else
		printf("Socket successfully created... \n");
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servaddr.sin_port=htons(port);
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))!=0)
	{
		printf("Conncetion failed... \n");
		exit(0);
	}
	else
		printf("Successfully connected... \n");
	func(sockfd);
	close(sockfd);
}