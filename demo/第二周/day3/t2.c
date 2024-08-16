#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <fcntl.h>
typedef struct
{
	long mtype;
	char mtext[20];
}msgbuf; 
int main()
{
	int msgid;
	msgbuf msg;
	
	int pid=fork();
	if(pid<0)
	{
		perror("fork");
		_exit(-1);
	}
	if(pid>0)
	{
		
		msgid=msgget(100,IPC_CREAT|0666);
		msg.mtype = 1;
		strcpy(msg.mtext,"nichou");
		msgsnd(msgid,&msg,(sizeof(msg)-sizeof(long)),0);
		wait(NULL);
		msgctl(msgid,IPC_RMID,NULL);
		_exit(0);
	}
	else
	{
		
		msgid=msgget(100,IPC_CREAT|0666);
		if(msgrcv(msgid,&msg,sizeof(msg)-sizeof(long),1,0)>0)
		{
			int len =strlen(msg.mtext);
			for(int i=len-1;i>=0;i--)
			{
				printf("%c",msg.mtext[i]);
			}
			printf("\n");
		}	
		_exit(0);
	}
}
