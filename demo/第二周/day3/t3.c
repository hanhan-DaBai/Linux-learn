#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <signal.h>

int flag=0;
void fun(int sig)
{
	static int cnt =0;
	cnt++;
	if(cnt>=10)
	{
		printf("child is alive\n");
		cnt=0;	
	}
	flag=0;
}
int main()
{

	int pid=fork();
	if(pid<0)
	{
		perror("fork");
		_exit(-1);
	}
	if(pid>0)
	{
		signal(2,SIG_IGN);
		signal(3,fun);
		while(1)
		{
			sleep(1);
			flag++;
			if(flag>2)
			{
				printf("child is die\n");
			}
		}
		
	}
	else
	{
		sleep(1);
		while(1)
		{
			kill(getppid(),3);
			sleep(1);
		}
		

		
		_exit(0);
	}
}
