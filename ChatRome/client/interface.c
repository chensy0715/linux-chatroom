#include "config.h"


int mainInterface()
{
   
		printf("-------------------------------------\n");
		printf("    welcome to chatroom～	      \n");
		printf("	   1.register                 \n");
		printf("	   2.login			      \n");
		printf("	   3.help			      \n");
		printf("	   4.exit			      \n");
		printf("-------------------------------------\n\n\n");
}


int helpInterface()
{
   
		printf("-------------------------------------\n");
		printf("         welcome to help page～	      \n");
		printf("	           ^_^                 \n");
		printf("        choose your operation in main page～     \n");
		printf("	           ^_^			      \n");
		printf("-------------------------------------\n\n\n");
}

/***************************************************
chat page
***************************************************/
void chatInterface(char userName[])
{
		printf("------------------------------------------\n");
		printf("Hi，%s                       \n", userName);
		printf("	     1. chat user online                 \n");
		printf("	     2. private chat                         \n");
		printf("	     3. group chat                         \n");
		printf("	     4. check history                 \n");
		printf("	     5. exit                         \n");
		printf("plase choose your operation～                    \n");
		printf("------------------------------------------\n\n\n");
}

