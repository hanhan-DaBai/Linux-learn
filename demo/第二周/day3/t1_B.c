#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#define FIFO1_PATH "./fifo1"
#define FIFO2_PATH "./fifo2"

int main()
{
	int pid=fork();
	if(pid<0)
	{
		perror("fork");
		_exit(-1);
	}
	if(pid>0)//B
	{
		wait(NULL);
		int fd = open(FIFO2_PATH,O_RDWR);
		if(fd<0)
		{
			perror("open");
			_exit(-2);
		}
		write(fd,"die",3);
		close(fd);
		printf("procee B exit\n");
		_exit(0);
	}
	else//b
	{
		char buf[100];
		printf("this is b\n");
		int fd =open(FIFO1_PATH,O_RDWR);
		if(fd<0)
		{
			perror("open");
			printf("procee is exit");
			_exit(-1);
			
		}
		if(read(fd,buf,100)>0)
		{
			printf("read:%s\n",buf);
		}
	}
}
