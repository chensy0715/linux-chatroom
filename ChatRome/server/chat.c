#include "config.h"

extern ListNode *userList;


int groupChat(Message *msg , int sockfd)
{
	ListNode *p;

	int ret;


	sqlite3 *db;
	sqlite3_stmt *stmt;
	const char *tail;

	char sql[128];


	char buf[MAX_LINE];

	Message message;
	memset(&message , 0 , sizeof(message));
	strcpy(message.sendName , (*msg).sendName);
	strcpy(message.recvName , (*msg).recvName);
	message.msgType = (*msg).msgType;


	p = userList;

	if(p->next == NULL)
	{

		message.msgType = RESULT;
		strcpy(message.content, stateMsg(ALL_NOT_ONLINE));
		memset(buf , 0 , MAX_LINE);
		memcpy(buf , &message , sizeof(message));
		send(sockfd , buf , sizeof(buf) , 0);
		return ALL_NOT_ONLINE;
	}//if

	else
	{
		strcpy(message.recvName , "");
		strcpy(message.content , (*msg).content);
		strcpy(message.msgTime , (*msg).msgTime);
		while(p!=NULL)
		{
			if(strcmp((p->user).userName , message.sendName) != 0)
			{
				memset(buf , 0 , MAX_LINE);
				memcpy(buf , &message , sizeof(message));
				send((p->user).sockfd , buf , sizeof(buf) , 0);
			}//else
			p = p->next;
		}//while

		ret = sqlite3_open(DB_NAME, &db);
		if(ret != SQLITE_OK)
		{
			printf("unable open database!\n");
			return FAILED;
		}//if

		memset(sql , 0 , sizeof(sql));
		sprintf(sql , "insert into Message(msgType , sendName , recvName , content , msgTime)\
				values(%d,'%s','%s','%s', '%s');",message.msgType , message.sendName ,
				message.recvName,message.content , message.msgTime);

		ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);
		if(ret != SQLITE_OK)
		{
			ret = sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return FAILED;
		}//if


		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);

		return SUCCESS;
	}//else
}


int personalChat(Message *msg , int sockfd)
{
	ListNode *p;

	int ret;


	sqlite3 *db;
	sqlite3_stmt *stmt;
	const char *tail;

	char sql[128];


	char buf[MAX_LINE];

	Message message;
	memset(&message , 0 , sizeof(message));
	strcpy(message.sendName , (*msg).sendName);
	strcpy(message.recvName , (*msg).recvName);
	message.msgType = (*msg).msgType;

	if(strcmp((*msg).sendName , (*msg).recvName) == 0)
	{
		printf("can not send to yourself！\n");

		message.msgType = RESULT;
		strcpy(message.content, stateMsg(MESSAGE_SELF));
		memset(buf , 0 , MAX_LINE);
		memcpy(buf , &message , sizeof(message));
		send(sockfd , buf , sizeof(buf) , 0);
		return MESSAGE_SELF;
	}//if


	p = userList;
	while(p != NULL && strcmp((p->user).userName , (*msg).recvName) != 0)
	{
		p = p->next;
	}//while

	if(p == NULL)
	{
		printf("user not online！\n");

		message.msgType = RESULT;
		strcpy(message.content, stateMsg(ID_NOT_ONLINE));
		memset(buf , 0 , MAX_LINE);
		memcpy(buf , &message , sizeof(message));
		send(sockfd , buf , sizeof(buf) , 0);
		return ID_NOT_ONLINE;
	}//if
	else{
		strcpy(message.content , (*msg).content);
		strcpy(message.msgTime , (*msg).msgTime);
		memset(buf , 0 , MAX_LINE);
		memcpy(buf , &message , sizeof(message));
		send((p->user).sockfd , buf , sizeof(buf) , 0);


		ret = sqlite3_open(DB_NAME, &db);
		if(ret != SQLITE_OK)
		{
			printf("unable open database!\n");
			return FAILED;
		}//if

		memset(sql , 0 , sizeof(sql));

		sprintf(sql , "insert into Message(msgType , sendName , recvName , content , msgTime)\                                                                               
				values(%d,'%s','%s','%s', '%s');",message.msgType , message.sendName ,
				message.recvName,message.content , message.msgTime);
				


		printf("%s\n" , sql);

		ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);
		if(ret != SQLITE_OK)
		{
			ret = sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return FAILED;
		}//if


		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);

		return SUCCESS;
	}//else
}


int viewUserList(Message *msg , int sockfd)
{
	ListNode *p;
	int ret;


	char buf[MAX_LINE];

	Message message;
	memset(&message , 0 , sizeof(message));
	strcpy(message.sendName , (*msg).sendName);
	strcpy(message.recvName , (*msg).recvName);
	message.msgType = (*msg).msgType;


	p = userList;
	if(p == NULL)
	{

		message.msgType = RESULT;
		strcpy(message.content, stateMsg(ALL_NOT_ONLINE));
		memset(buf , 0 , MAX_LINE);
		memcpy(buf , &message , sizeof(message));
		send(sockfd , buf , sizeof(buf) , 0);
		return ALL_NOT_ONLINE;
	}//if
	else{

		strcpy(message.content , "");
		while(p!=NULL)
		{
			strcat(message.content , "\t");
			strcat(message.content , (p->user).userName);

			p = p->next;
		}//while
		memset(buf , 0 , MAX_LINE);
		memcpy(buf , &message , sizeof(message));
		send(sockfd , buf , sizeof(buf) , 0);
		printf("check online list：%s\n", message.content);
	}
	return SUCCESS;
}



int viewRecords(Message *msg , int sockfd)
{
	int ret;

	char buf[MAX_LINE] , record[MAX_LINE];


	sqlite3 *db;
	char *errmsg = NULL;
	char **dbRet;
	int nRow , nCol , i , j , idx;


	char sql[128];


	Message message;
	memset(&message , 0 , sizeof(message));
	strcpy(message.sendName , (*msg).sendName);

	if(strcmp( (*msg).recvName , "all") == 0)
		strcpy(message.recvName , "");
	else
		strcpy(message.recvName , (*msg).recvName);
	message.msgType = (*msg).msgType;


	ret = sqlite3_open(DB_NAME, &db);
	if(ret != SQLITE_OK)
	{
		printf("unable open database.\n");

		message.msgType = RESULT;
		strcpy(message.content, stateMsg(FAILED));
		memset(buf , 0 , MAX_LINE);
		memcpy(buf , &message , sizeof(message));
		send(sockfd , buf , sizeof(buf) , 0);

		return FAILED;
	}//if


	memset(sql , 0 , sizeof(sql));
	if(strcmp(message.recvName , "") == 0)
		sprintf(sql , "select * from Message where recvName='%s' order by msgTime;",message.recvName);
	else
		sprintf(sql , "select * from Message where sendName='%s' and recvName='%s' or sendName='%s' and recvName='%s' order by msgTime;",message.sendName , message.recvName , message.recvName , message.sendName);


	ret = sqlite3_get_table(db , sql , &dbRet , &nRow , &nCol , &errmsg);
	/*查询不成功*/
	if(ret != SQLITE_OK)
	{
		sqlite3_close(db);
		printf("database select fail!\n");

		message.msgType = RESULT;
		strcpy(message.content, stateMsg(FAILED));
		memset(buf , 0 , MAX_LINE);
		memcpy(buf , &message , sizeof(message));
		send(sockfd , buf , sizeof(buf) , 0);
		return FAILED;
	}//if


	idx = nCol;
	for(i=0; i<nRow; ++i)
	{
		memset(record , 0 , MAX_LINE);
		sprintf(record , "%s\t%s\n\t%s\n\n", dbRet[idx+1] , dbRet[idx+4] , dbRet[idx+3]);
		//printf("第%d条记录:%s\n",i,record);
		idx = idx + nCol;
		strcat(message.content , record);
	}//for
	message.content[strlen(message.content)-1] = '\0';

	sqlite3_close(db);


	memset(buf , 0 , MAX_LINE);
	memcpy(buf , &message , sizeof(message));
	send(sockfd , buf , sizeof(buf) , 0);
	return SUCCESS;
}



void enterChat(int *fd)
{
	int n,ret,sockfd;
	User user;

	char buf[MAX_LINE];
	memset(buf , 0 , MAX_LINE);


	Message message;
	memset(&message , 0 , sizeof(message));

	sockfd = *fd;

	while(1)
	{

		n = recv(sockfd , buf , sizeof(buf)+1 , 0);
		//printf("enterChat n = %d\n" , n);
		if(n == 0)
		{

			close(sockfd);
			return ;
		}//if
		else{
			memcpy(&message , buf , sizeof(buf));
			//printf("server msgType = %d\n" , message.msgType);
			switch(message.msgType)
			{
			case GROUP_CHAT:
				{
					printf("group chat from %s...！\n", message.sendName);

					ret = groupChat(&message , sockfd);
					printf("group chat：%s\n", stateMsg(ret));
					break;
				}
			case PERSONAL_CHAT:
				{
					printf("private chat from %s.....！\n", message.sendName);

					ret = personalChat(&message , sockfd);
					printf("私聊：%s\n", stateMsg(ret));
				}
				break;
			case VIEW_USER_LIST:
				{
					printf("check online list  requireent from %s.....！\n", message.sendName);

					ret = viewUserList(&message , sockfd);
					printf("check list online：%s\n", stateMsg(ret));
					break;
				}
			case VIEW_RECORDS:
				{
					printf("chat history from %s....！\n", message.sendName);
					ret = viewRecords(&message , sockfd);
					printf("check history：%s\n", stateMsg(ret));
					break;
				}
			case EXIT:
				{
					
					printf("user %s exit ！\n", message.sendName);
					memset(&user , 0 , sizeof(user));
					strcpy(user.userName , message.sendName);
					deleteNode(userList , &user);
					close(sockfd);
					return;
				}
			default:
				break;
			}//switch
		}//else
	}//while
	return ;
}
