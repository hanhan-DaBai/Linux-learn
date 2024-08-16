#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#define FIFO1_PATH "./fifo1"
#define FIFO2_PATH "./fifo2"
void funa(int sig)
{
	printf("\nrecv ctrl+c signal\n");
	int fd=open(FIFO1_PATH,O_RDWR);
	if(fd<0)
	{
		perror("open");
		_exit(-2);
	}
	write(fd,"nihao",5);
	close(fd);
	printf("procee a exit\n");
	_exit(0);
	
}

int main()
{

	if(mkfifo(FIFO1_PATH,0666)<0)
	{
		perror("mkfifo");
		if(errno !=17)
		{_exit(-1);}
	}
	if(mkfifo(FIFO2_PATH,0666)<0)
	{
		if(errno !=17)
		{_exit(-1);}
		perror("mkfifo");
		
	}
	int pid=fork();
	if(pid<0)
	{
		perror("fork");
		_exit(-1);
	}
	if(pid>0)//A
	{
		char buf[100];
		signal(SIGINT,SIG_IGN);

		int fd =open(FIFO2_PATH,O_RDWR);
		if(fd<0)
		{
			perror("open");
			_exit(-2);
		}
		if(read(fd,buf,100)>0)
		{
			wait(NULL);
			printf("procee A is exit\n");
			unlink(FIFO1_PATH);
			unlink(FIFO2_PATH);
			_exit(0);
		}
	}else//a
	{
		signal(SIGINT,funa);
		while(1)
		{
			printf("this is a\n");
			
			sleep(1);
		}
	}
	
}
