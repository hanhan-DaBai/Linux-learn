#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<errno.h>

int main(int argc,char const *argv[])
{
	int fd;
	char*pipe_path="./myfifo";
	//打印错误信息，当不是文件已存在的时候关闭进程
	if(mkfifo(pipe_path,0664) !=0)
	{
		perror("mkfifo");
		if(errno != 17)
		{
				exit(EXIT_FAILURE);
		}
		
		
	}
	
	//对有名管道的特殊文件创建fd
	fd = open(pipe_path,O_WRONLY);
	if(fd==-1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	char buf[100];
	ssize_t read_num;
	//读取控制台数据写入管道中
	while((read_num=read(STDIN_FILENO,buf,10))>0)
	{
		write(fd,buf,read_num);
	}
	if(read_num<0)
	{
		perror("read");
		close(fd);
		exit(EXIT_FAILURE);
	}
	
	printf("发送数据完成进程终止\n");
	close(fd);
	//释放管道
	//清除对应的特殊文件
	if(unlink(pipe_path)==-1)
	{
		perror("unlink");
	}
	return 0;
}
