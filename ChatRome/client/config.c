
#include "config.h"


char *stateMsg(int stateRet)
{
	switch(stateRet)
	{
	case EXCEED:
		return "reach the limitation of server！\n";
		break;
    case SUCCESS: 
		return "operation sucess！\n";
		break;
	case FAILED:  
		return "operation fail！\n";
		break;    
	case DUPLICATEID: 
		return "duplicative username！\n";
		break;	
	case INVALID:	
		return "invalid enter！\n";
		break;    
	case ID_NOT_EXIST: 
		return "account not exist！\n";
		break;
    case WRONGPWD: 
		return "invalid password！\n";
		break;
	case ALREADY_ONLINE:
		return "user online！\n";
		break;
	case ID_NOT_ONLINE:
		return "user not online！\n";
		break;
	case ALL_NOT_ONLINE:
		return "none online！\n";
		break;
	case MESSAGE_SELF:  
		return "can not send to yourself\n";	
		break;
	default:
		return "unknown operation result！\n";
		break;
	}//switch
};

/*************************************

**************************************/
void copyUser(User *user1 , User *user2)
{
	strcpy((*user1).userName , (*user2).userName);
	strcpy((*user1).password , (*user2).password);
	(*user1).userAddr = (*user2).userAddr;
	(*user1).sockfd = (*user2).sockfd;
	(*user1).speak = (*user2).speak;
	strcpy((*user2).registerTime , (*user2).registerTime);

}
