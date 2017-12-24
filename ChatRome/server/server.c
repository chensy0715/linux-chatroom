#include "config.h"


ListNode *userList = NULL;

/*********************************************
main
**********************************************/
int main(void)
{

	int i , n , ret , maxi , maxfd , listenfd , connfd , sockfd;

	socklen_t clilen;

	pthread_t pid;


	int opt = 1;


	struct sockaddr_in servaddr , cliaddr;


	fd_set rset , allset;

	int nready , client_sockfd[FD_SETSIZE];


	Message message;

	char buf[MAX_LINE];


	User user;


	if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error.\n");
		exit(1);
	}//if


	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons((unsigned short)6001);


	setsockopt(listenfd , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt));

	if(bind(listenfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
	{
		perror("bind error.\n");
		exit(1);
	}//if


	if(listen(listenfd , LISTENEQ) < 0)
	{
		perror("listen error.\n");
		exit(1);
	}//if


	maxfd = listenfd;
	maxi = -1;
	for(i=0; i<FD_SETSIZE; ++i)
	{
		client_sockfd[i] = -1;
	}//for

	FD_ZERO(&allset);


	FD_SET(listenfd , &allset);


	while(1)
	{
		rset = allset;

		nready = select(maxfd+1 , &rset , NULL , NULL , 0);


		if(FD_ISSET(listenfd , &rset))
		{

			clilen = sizeof(cliaddr);
			if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0)
			{
				perror("accept error.\n");
				exit(1);
			}//if

			printf("server: got connection from %s\n", inet_ntoa(cliaddr.sin_addr));


			for(i=0; i<FD_SETSIZE; ++i)
			{
				if(client_sockfd[i] < 0)
				{
					client_sockfd[i] = connfd;
					break;
				}//if
			}//for

			if(i == FD_SETSIZE)
			{
				perror("too many connection.\n");
				exit(1);
			}//if


			FD_SET(connfd , &allset);


			if(connfd > maxfd)
				maxfd = connfd;

			/*max index in client_sockfd[]*/
			if(i > maxi)
				maxi = i;

			/*no more readable descriptors*/
			if(--nready <= 0)
				continue;
		}//if

		for(i=0 ; i<=maxi ; ++i)
		{
			if((sockfd = client_sockfd[i]) < 0)
				continue;

			if(FD_ISSET(sockfd , &rset))
			{

				if(--nready < 0)
					break;
				pthread_create(&pid , NULL , (void *)handleRequest , (void *)&sockfd);

			}//if

			FD_CLR(sockfd , &allset);
			client_sockfd[i] = -1;
		}//for
	}//while

	close(listenfd);
	return 0;
}


void* handleRequest(int *fd)
{
	int sockfd , ret , n;

	Message message;

	char buf[MAX_LINE];

	sockfd = *fd;

	memset(buf , 0 , MAX_LINE);
	memset(&message , 0 , sizeof(message));


	n = recv(sockfd , buf , sizeof(buf)+1 , 0);
	if(n <= 0)
	{

		fflush(stdout);
		close(sockfd);
		*fd = -1;
		printf("exit requirement from %s！\n", inet_ntoa(message.sendAddr.sin_addr));
		return NULL;
	}//if
	else{
		memcpy(&message , buf , sizeof(buf));

		switch(message.msgType)
		{
		case REGISTER:
			{
				printf("register requirement from %s！\n", inet_ntoa(message.sendAddr.sin_addr));
				ret = registerUser(&message , sockfd);
				memset(&message , 0 , sizeof(message));
				message.msgType = RESULT;
				message.msgRet = ret;
				strcpy(message.content , stateMsg(ret));
				memset(buf , 0 , MAX_LINE);
				memcpy(buf , &message , sizeof(message));
				/*发送操作结果消息*/
				send(sockfd , buf , sizeof(buf) , 0);
				printf("register：%s\n", stateMsg(ret));
				close(sockfd);
				*fd = -1;
				return NULL;
				break;
			}//case
		case LOGIN:
			{
				printf("login requirement from %s.....！\n", inet_ntoa(message.sendAddr.sin_addr));
				ret = loginUser(&message , sockfd);
				memset(&message , 0 , sizeof(message));
				message.msgType = RESULT;
				message.msgRet = ret;
				strcpy(message.content , stateMsg(ret));
				memset(buf , 0 , MAX_LINE);
				memcpy(buf , &message , sizeof(message));

				send(sockfd , buf , sizeof(buf) , 0);
				printf("login：%s\n", stateMsg(ret));

				enterChat(&sockfd);
				break;
			}//case
		default:
			printf("unknown operation.\n");
			break;
		}//switch
	}//else

	close(sockfd);
	*fd = -1;
	return NULL;
}
