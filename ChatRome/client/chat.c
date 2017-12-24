#include "config.h"



void recvMsg(int *sockfd)
{
	int connfd = *sockfd;
	int nRead;

	char buf[MAX_LINE] , str[MAX_LINE];
	Message message;
	
	time_t timep;

	printf("^_^ receiving chat message～\n");
	while(1)
	{
		
		nRead = recv(connfd , buf , sizeof(message) , 0);
	
		if(nRead <= 0)
		{
			printf("invalid logout, re-enter again\n");
			close(connfd);
			exit(0);
		}//if

		memset(&message , 0 , sizeof(message));
		memcpy(&message , buf , sizeof(buf));

		switch(message.msgType)
		{
		case VIEW_USER_LIST:
			printf("user online：\n %s\n", message.content);
			break;
		case PERSONAL_CHAT:
			sprintf(str , "%s \t %s \tto you： %s\n", message.sendName , message.msgTime , message.content);
			printf("\n%s\n", str);
			break;
		case GROUP_CHAT:
			sprintf(str , "%s \t %s \tsend group message： %s\n", message.sendName , message.msgTime , message.content);
			printf("\n%s\n", str);
			break;
		case VIEW_RECORDS:
			if(strcmp(message.recvName , "") == 0)
				printf("group message history：\n\n");			
			else
				printf("your message history to %s ... ：\n\n", message.recvName);
			printf("%s\n" , message.content);
			break;
		case RESULT:
			printf("your operation result：%s\n", message.content);
		default:
			break; 
		}//switch
	}//while	
}



/*************************************************/
void enterChat(User *user , int sockfd)
{
	int choice , ret;
	char c , buf[MAX_LINE] , str[MAX_LINE];
	Message message;	
	time_t timep;

	pthread_t pid;	
	

	ret = pthread_create(&pid , NULL , (void *)recvMsg , (void *)&sockfd);
	if(ret != 0)
	{
		printf("software invalid, login again！\n");
		memset(&message , 0 , sizeof(message));
		strcpy(message.sendName , (*user).userName);
		message.msgType = EXIT;
		send(sockfd , buf , sizeof(buf) , 0);
		close(sockfd);
		exit(1);
	}
	
	setbuf(stdin, NULL);
	

	while(1)
	{
		memset(&message , 0 , sizeof(message));
		strcpy(message.sendName , (*user).userName);
		memset(&str , 0 , MAX_LINE);
		memset(buf , 0 , MAX_LINE);
		
		usleep(100000);

		
		chatInterface((*user).userName);
		setbuf(stdin,NULL); 
		scanf("%d",&choice);
		while(choice != 1 && choice != 2 && choice != 3 && choice !=4 && choice != 5)
		{
			printf("invalid operation, re-enter again\n");
			setbuf(stdin,NULL); 
			scanf("%d",&choice);
			setbuf(stdin,NULL);
		}//while

		switch(choice)
		{
		case 1: 
			message.msgType = VIEW_USER_LIST;
			memcpy(buf , &message , sizeof(message));
			send(sockfd , buf , sizeof(buf) , 0);						
			break;	
		case 2:
			message.msgType = PERSONAL_CHAT;
			printf("send message to：\n");
			setbuf(stdin , NULL);
			scanf("%s" , str);
			strcpy(message.recvName , str);

			printf("message content：\n");
			setbuf(stdin , NULL);			
			fgets(message.content , MAX_LINE , stdin);
			(message.content)[strlen(message.content) - 1] = '\0';

			
			time(&timep);
			strcpy(message.msgTime , ctime(&timep));
			memcpy(buf , &message , sizeof(message));
			send(sockfd , buf , sizeof(buf) , 0);
			break;
		case 3: 
			message.msgType = GROUP_CHAT;
			strcpy(message.recvName , "");
			
			printf("enter your meesage：\n");
			setbuf(stdin , NULL);			
			fgets(message.content , MAX_LINE , stdin);
			(message.content)[strlen(message.content) - 1] = '\0';

			
			time(&timep);
			strcpy(message.msgTime , ctime(&timep));
			memcpy(buf , &message , sizeof(message));
			send(sockfd , buf , sizeof(buf) , 0);
			break;
		case 4: 
			message.msgType = VIEW_RECORDS;			
			printf("plase enter who you want to check：\n");
			setbuf(stdin , NULL);
			scanf("%s" , str);
			strcpy(message.recvName , str);			
			memcpy(buf , &message , sizeof(message));
			send(sockfd , buf , sizeof(buf) , 0);
			break;
		case 5: 
			message.msgType = EXIT;
			memcpy(buf , &message , sizeof(message));
			send(sockfd , buf , sizeof(buf) , 0);
			close(sockfd);
			exit(0);
		default: 	
			break;
		}//switch
	}//while
	//close(sockfd);
}


