#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define max 100
#define port 8080
int main()
{
	int listenfd,udpfd,connfd,maxfdp1,len,nready;
	struct sockaddr_in servaddr,cli;
	int childpid,n;
	char buff[max];
	fd_set rset;
	// Create listening TCP socket
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("TCP socket creation failed...\n");
		exit(0);
	}
	else
		printf("TCP socket successfully created...\n");

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servaddr.sin_port=htons(port);

	// Bind server address to TCP socket
	if((bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)))==-1)
	{
		printf("TCP socket binding failed...\n");
		exit(0);
	}
	else
		printf("TCP socket successfully binded...\n");
	if(listen(listenfd,5)==-1)
	{
		printf("Listening failed...\n");
		exit(0);
	}
	else
		printf("Successfully listening...\n");
	
	// Create UDP socket
	if((udpfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		printf("UDP socket creation failed...\n");
		exit(0);
	}
	else
		printf("UDP socket successfully created...\n");
	// Bind server address to UDP socket
	if(bind(udpfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
	{
		printf("UDP socket binding failed...\n");
		exit(0);
	}
	else
		printf("UDP socket successfully binded...\n");
	// clear the descriptor set
	FD_ZERO(&rset);
	//get max descriptor
	maxfdp1=(listenfd>udpfd?listenfd:udpfd)+1;
	for(;;)
	{
		FD_SET(listenfd,&rset);
		FD_SET(udpfd,&rset);
		nready=select(maxfdp1,&rset,NULL,NULL,NULL);
		if(FD_ISSET(listenfd,&rset))
		{
			len=sizeof(cli);
			connfd=accept(listenfd,(struct sockaddr*)&cli,&len);
			if((childpid=fork())==0)
			{
				close(listenfd);
				bzero(buff,max);
				read(connfd,buff,sizeof(buff));
				printf("Message from client:%s\n",buff);
				bzero(buff,max);
				n=0;
				printf("Enter message for client:");
				while((buff[n++]=getchar())!='\n');
				write(connfd,buff,sizeof(buff));
				close(connfd);
				exit(0);
			}
			close(connfd);
		}
		else
			if(FD_ISSET(udpfd,&rset))
			{
				len=sizeof(cli);
				bzero(buff,max);
				recvfrom(udpfd,buff,sizeof(buff),0,(struct sockaddr*)&cli,&len);
				printf("Message from client:%s\n",buff);
				bzero(buff,max);
				n=0;
				printf("Enter message for client:");
				while((buff[n++]=getchar())!='\n');
				sendto(udpfd,buff,sizeof(buff),0,(const struct sockaddr*)&cli,len);
			}
	}
}