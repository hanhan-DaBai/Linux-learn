#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void fun(int sig)
{
	printf("sig:%d\n",sig);
	//signal(SIGINT,SIG_DFL);
}
int main()
{
	printf("%d\n",getpid());
	signal(SIGINT,fun);
	sleep(10);
}
