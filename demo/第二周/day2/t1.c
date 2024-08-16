#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>


int main(int argc,char const* argv[])
{
	int pid;
	//定义管道数组
	int pipefd[2];
	//没有参数传入
	if(argc !=2)
	{		
		fprintf(stderr,"%s:请填写需要传递的信息\n",argv[0]);
		exit(EXIT_FAILURE);	
	}
	//创建管道
	if(pipe(pipefd)==-1)
	{
		perror("创建管道失败");
		exit(EXIT_FAILURE);
	}
	//复制父子进程
	pid=fork();
	//进程创建失败
	if(pid<0)
	{
		perror("邀请新学员失败！");
		exit(EXIT_FAILURE);
	}
	
	if(pid==0)
	{
		//子进程
		//关闭写端
		close(pipefd[1]);
		char str[100] ={0};
		sprintf(str,"新学员%d接收的信息\n",getpid());
		write(STDOUT_FILENO,str,sizeof(str));
		char buf;
		//从内核读
		while(read(pipefd[0],&buf,1))
		{
			write(STDOUT_FILENO,&buf,1);
		}
		write(STDOUT_FILENO,"\n",1);
		//关闭读端
		close(pipefd[0]);
		exit(EXIT_SUCCESS);
	}else
	{
		//父进程 写入管道数据
		//关闭读端
		close(pipefd[0]);
	
		printf("老学员%d对新学员传递的信息:\n",getpid());
		//把传入的参数写入内核
		write(pipefd[1],argv[1],strlen(argv[1]));
		//关闭写端
		close(pipefd[1]);
		//等待子进程关闭
		waitpid(pid,NULL,0);
		exit(EXIT_SUCCESS);
	}
	
	return 0;
	
}
