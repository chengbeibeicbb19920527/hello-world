#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<pthread.h>


#define PORT 5000
#define BACKLOG 10
#define LENGTH 512

void rec(void*);

int main()
{
	int sockfd;
	pthread_t thread1;
	int nsockfd;
	int num;
	int ret;
	int sin_size;
	char sdbuf[LENGTH];
	struct sockaddr_in addr_local;
	struct sockaddr_in addr_remote;
	char sendstr[16] = {"123456789abcde"};

	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("ERROR:Failed to obtain Socket Despcritor.\n");
		return(0);
	}
	else
	{
		printf("OK:Obtain Socket Despcritor successfully.\n");
	}
	addr_local.sin_family = AF_INET;
	addr_local.sin_port = htons(PORT);
	addr_local.sin_addr.s_addr = INADDR_ANY;
	bzero(&(addr_local.sin_zero),8);

	if(bind(sockfd,(struct sockaddr*)&addr_local,sizeof(struct sockaddr))==-1)
	{
		printf("ERROR:Failed to bind Port %d.\n",PORT);
		return(0);
	}
	else
	{
		printf("OK:Bind the Pore %d successfully.\n",PORT);
	}

	if(listen(sockfd,BACKLOG)==-1)
	{
		printf("ERROR:Failed ti listen Port.\n",PORT);
		return(0);
	}
	else
	{
		printf("OK:Listening the Port %d successfully.\n",PORT);
	}


	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if((nsockfd = accept(sockfd,(struct sockaddr*)&addr_remote,&sin_size))==-1)
		{
			printf("ERROE:Obtain new Socket Despcritor error.\n");
			continue;
		}
	else
	{
		printf("OK:Server has got connet from %s.\n",inet_ntoa(addr_remote.sin_addr));
	}
	ret = pthread_create(&thread1,PTHREAD_CREATE_JOINABLE,(void*)rec,(void*)nsockfd);
	//{
	//	if(ret = -1)
	//	{
	//		perror("pthread_create");
	//		exit(-1);
	//	}
	//}

	if(!fork())
	{
		printf("Youcan enter string,and press'exit' to end the connet.\n");
		while(strcmp(sdbuf,"exit")!=0)
		{
			scanf("%s",sdbuf);
			if((num = send(nsockfd,sdbuf,strlen(sdbuf),0))==-1)
			{
				printf("ERROR:failed to sent string.\n");
				close(nsockfd);
				exit(1);
			}
			printf("OK:Sent %d bytes successful,please enter again.\n",num);
		}
	}
	close(nsockfd);
	while(waitpid(-1,NULL,WNOHANG)>0);
	}
}

void rec(void* nsockfd)
{
	int fd = (int)nsockfd;
	int num;
	//printf("%d\n",fd);
	char buf[LENGTH];
	while(1)
	{
		bzero(buf,LENGTH);
		num = read(fd,buf,LENGTH);
		if(strlen(buf)>0)
		{

			printf("you have a message:%s\n",buf);
		}
	}
}
