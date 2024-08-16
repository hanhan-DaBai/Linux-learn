#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	int fd[2];
	int pid,cnt;
	if(pipe(fd)<0)
	{
		perror("pipe create failed!\n");
		exit(-1);
	}
	
	pid=fork();
	if(pid<0)
	{
		perror("process create failed!\n");
		exit(-2);
	}
	if(pid>0)
	{
		cnt =0;
		while(1)
		{
			printf("father:%d\n",cnt);
			write(fd[1],&cnt,sizeof(cnt));
			sleep(1);
			if(cnt>100)
			{
				write(fd[1],&cnt,sizeof(cnt));
				break;
			}
			cnt+=2;
		}
		
		wait(NULL);
		_exit(0);
	}
	else
	{
		while(1)
		{
			
			read(fd[0],&cnt,sizeof(cnt));
			if(cnt>100)break;
			printf("child cnt =%d\n",cnt+1);
		}
		
		_exit(0);
	}	
}
