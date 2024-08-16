#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<time.h>
#include<stdlib.h>

#define MSG_LEN 100
typedef struct{
	long mtype;
	char mtext[MSG_LEN];
}msgbuf;

int main()
{
	int pid;
	int msg_id =msgget(1000,IPC_CREAT|0666);
	msgbuf msg;
	if(msg_id<0)
	{
		perror("msg get failed\r\n");
		_exit(-1);
	}
	pid= fork();
	if(pid<0)
	{
		perror("fork failed\r\n");
		_exit(-2);
	}
	if(pid>0)
	{
		while(1)
		{
			srand(time(NULL));
			int num=rand()%10+1;
			
			msg.mtype=2;
			msg.mtext[0]=num;
			
			if(!msgsnd(msg_id,&msg,MSG_LEN,0))
			{
				printf("made %d baozi \n",num);
			}
			sleep(1);
		}
	}
	else
	{
		while(1)
		{
			if(msgrcv(msg_id,&msg,MSG_LEN,2,0)>0)
			{
				printf("eat %d\n",msg.mtext[0]);
			}
		}
	
	}
	wait(NULL);
}
