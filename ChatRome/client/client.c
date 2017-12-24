#include "config.h"


/*********************************************
main

**********************************************/
int main(int argc , char *argv[])
{
	int sockfd , choice , ret; 
	struct sockaddr_in servaddr;
	struct hostent *host;
	
	
	Message message;
	
	char buf[MAX_LINE];

	/*UserInfo*/
	User user;
	strcpy(user.userName , "***");
	user.speak = 1;

	
   
   // if(argc != 2)
   // {
   //     perror("usage:tcpcli <IPaddress>");
    //    exit(1);
   // }//if

	while(1)
	{
		
		if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
		{
			perror("socket error");
			exit(1);
		}//if

		
		bzero(&servaddr , sizeof(servaddr));		
		servaddr.sin_family = AF_INET;			
		servaddr.sin_port = htons((unsigned short)6001);			
		//servaddr.sin_addr = *((struct in_addr *)host->h_addr);		
		if(inet_pton(AF_INET , "18.221.180.99" , &servaddr.sin_addr) < 0)
		{
			printf("inet_pton error for %s\n",argv[1]);
			exit(1);
		}//if

		
		if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
		{
			perror("connect error");
			exit(1);
		}//if	

			
		mainInterface();	
		setbuf(stdin,NULL); 
		scanf("%d",&choice);
		setbuf(stdin,NULL);
		while(choice != 1 && choice != 2 && choice != 3 && choice !=4)
		{
			printf("can not find this order, retype again\n");
			setbuf(stdin,NULL); 
			scanf("%d",&choice);
			setbuf(stdin,NULL);
		}//while

				
		switch(choice)
		{		
		case REGISTER:		
			memset(&message , 0 , sizeof(message));
			memset(buf , 0 , MAX_LINE);		
			message.msgType = REGISTER;
			strcpy(message.content , "");
			message.sendAddr = servaddr;
			
			memcpy(buf , &message , sizeof(message));	
			send(sockfd , buf , sizeof(buf) , 0);	
			registerUser(sockfd);
			//goto sign;
			break;
		case LOGIN:		
			memset(&message , 0 , sizeof(message));
			memset(buf , 0 , MAX_LINE);
			message.msgType = LOGIN;
			strcpy(message.content , "");
			message.sendAddr = servaddr;
			
			memcpy(buf , &message , sizeof(message));
			send(sockfd , buf , sizeof(buf) , 0);
			loginUser(sockfd);					
			break;	
		case HELP:		
            helpInterface(); 
			//goto sign;                                                                
			break;
		case EXIT:
			close(sockfd);
			printf("exit chatroom!\n");
			exit(0);	
			break;
		default:
			printf("unknown operation.\n");
			//goto sign;
			break;	
		}//switch	
	}//while	
	close(sockfd);
	return 0;	
}
