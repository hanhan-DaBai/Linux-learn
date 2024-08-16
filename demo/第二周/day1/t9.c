#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int n=0;
	scanf("%d",&n);
	printf("main's id %d\n",getpid());
	for(int i=0;i<n;i++)
	{int pid=1;
	if(pid!=0)	pid=fork();
		if(pid==0)
		{
			printf("child's id %d\n",getpid());
			printf("father's id %d\n",getppid());
			exit(0);
		}
	}
	for(int i=0;i<n;i++)
	{
		wait(NULL);
	}
}
