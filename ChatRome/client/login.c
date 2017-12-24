#include "config.h"


int loginUser(int sockfd)
{
	int ret;
	
	User user;
	char buf[MAX_LINE];
	Message message;
	
	printf("enter username：\n");
	memset(user.userName , 0 , sizeof(user.userName));
	scanf("%s" , user.userName);
	printf("user.UserName = %s\n" , user.userName);
	
	printf("enter password：\n");
	memset(user.password , 0 , sizeof(user.password));
	scanf("%s" , user.password);
	printf("user.password = %s\n" , user.password);

	
	memset(buf , 0 , MAX_LINE);
	memcpy(buf , &user , sizeof(user));
	send(sockfd , buf , sizeof(buf) , 0);

	
	memset(buf , 0 , MAX_LINE);
	recv(sockfd , buf , sizeof(buf) , 0);
	memset(&message , 0 , sizeof(message));
	memcpy(&message , buf , sizeof(buf));
	
	printf("%s\n",message.content);
	
	
	if(SUCCESS == message.msgRet)
	{
		enterChat(&user , sockfd);
	}//if
	return message.msgRet;
}


