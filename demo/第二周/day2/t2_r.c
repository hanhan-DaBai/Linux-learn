#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char const *argv[])
{
	int fd;
	char*pipe_path="./myfifo";
	
	if(mkfifo(pipe_path,0664) !=0)
	{
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}
	
	//对有名管道的特殊文件创建fd
	fd = open(pipe_path,O_RDONLY);
	if(fd==-1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	char buf[100];
	ssize_t read_num;
	//读取管道数据打印控制台中
	while((read_num=read(fd,buf,10))>0)
	{
		write(STDOUT_FILENO,buf,read_num);
	}
	if(read_num<0)
	{
		perror("read");
		close(fd);
		exit(EXIT_FAILURE);
	}
	else
	printf("接收数据完成进程终止\n");
	close(fd);
	return 0;
}
