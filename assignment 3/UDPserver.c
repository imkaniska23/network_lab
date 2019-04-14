#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define max 100
#define port 8080
void func(int sockfd)
{
	char buff[max];
	int n,len;
	struct sockaddr_in cli;
	len=sizeof(cli);
	for(;;)
	{
		printf("Message from client:");
		bzero(buff,max);
		recvfrom(sockfd,buff,sizeof(buff),0,(struct sockadd*)&cli,&len);
		printf("%s\n",buff);
		int i,j=0;
		for(i=0;buff[i]!='\n';i++)
			;
		i--;
		while(j<i)
		{
			if(buff[j]==buff[i])
			{
				j++;
				i--;
			}
			else
				break;
		}
		if(j>=i)
		{
			bzero(buff,max);
			char *ans="It is a palindrome";
			strcpy(buff,ans);
		}
		else
		{
			bzero(buff,max);
			char *ans="It is not a palindrome";
			strcpy(buff,ans);
		}
		sendto(sockfd,buff,sizeof(buff),0,(const struct sockadd*)&cli,len);
		if(strncmp(buff,"exit",4)==0)
		{
			printf("Server exiting...\n");
			break;
		}
	}
}
int main()
{
	int sockfd;
	struct sockaddr_in servaddr, cli;
	memset(&servaddr,0,sizeof(servaddr));
	memset(&servaddr,0,sizeof(cli));
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created...\n");
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servaddr.sin_port=htons(port);
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
	{
		printf("Binding failed...\n");
		exit(0);
	}
	else
		printf("Binding successfull...\n");
	func(sockfd);
	close(sockfd);
}