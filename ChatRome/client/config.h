/*******************************************************************************

********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h> 

#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>

#include <sqlite3.h>


#ifndef FD_SETSIZE
#define FD_SETSIZE 256
#endif

#define MAX_LINE  8192
#define PORT  8888
#define LISTENEQ  6000
#define IPADDR    "127.0.0.1"
 

#define DB_NAME "/home/chen/Downloads/ChatRome_select/chatRome.db"


enum Flag{
	YES,	
	NO		
};


enum MessageType{	
	REGISTER = 1,		
	LOGIN,		
	HELP,		
	EXIT,				
	VIEW_USER_LIST,		
	GROUP_CHAT,		
	PERSONAL_CHAT,		
	VIEW_RECORDS,	
	RESULT,			
	UNKONWN				
};


enum StateRet{
    EXCEED, 
    SUCCESS, 
	FAILED,  
    DUPLICATEID, 
	INVALID,	
    ID_NOT_EXIST, 
    WRONGPWD,
	ALREADY_ONLINE,		
	ID_NOT_ONLINE,	
	ALL_NOT_ONLINE, 	
	MESSAGE_SELF   
};



typedef struct _Message{
	char content[2048];		
	int msgType;	
	int msgRet;		
	struct sockaddr_in sendAddr; 
	struct sockaddr_in recvAddr;
	char sendName[20]; 
	char recvName[20]; 
	char msgTime[20];  
}Message;


typedef struct _User{
	char userName[20]; 		
	char password[20];
	struct sockaddr_in userAddr;	
	int sockfd;			
	int speak;			
	char registerTime[20];	
}User;


typedef struct _ListNode{
	User user;
	struct _ListNode *next;
}ListNode;



ListNode *userList;

extern char *stateMsg(int stateRet);
extern void copyUser(User *user1 , User *user2);

