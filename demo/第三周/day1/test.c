#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
// 宏定义端口号
#define PROT 8083
int server;

void handle_signal(int sig)
{
    if(sig==SIGINT)
    {
        printf("\nServer shutting down...\n");
        close(server); // 关闭服务器套接字
        exit(0);
    }
}
int main()
{
    char buf[1024];
    struct sockaddr_in sock_addr;
    fd_set readfds,fds;
    int max_fd =0;
    //创建套接字
    server=socket(AF_INET,SOCK_STREAM,0);
    if(server<0)
    {
        perror("socket");
        _exit(EXIT_FAILURE);
    }
    //接收CTRL+C
    signal(SIGINT,handle_signal);
    //配置并绑定ip与端口号
    sock_addr.sin_family=AF_INET;
    sock_addr.sin_port=htons(PROT);
    sock_addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(server,(struct sockaddr *)&sock_addr,sizeof(sock_addr))<0)
    {
        perror("bind");
        _exit(EXIT_FAILURE);
    }
    //监听套接字
    if(listen(server,100)<0)
    {
        perror("listen");
        _exit(EXIT_FAILURE);
    }

    //初始化fds，readfds，max_FD。把server放入fds中
    max_fd =server;
    FD_ZERO(&fds);
    FD_ZERO(&readfds);
    FD_SET(server,&fds);

    while (1)
    {
        //遍历文件描述符，把fd放入readfds中
        for(int fd=0;fd<1024;fd++)
        {
            if(FD_ISSET(fd,&fds))
            {
                FD_SET(fd,&readfds);
                if(fd>max_fd)max_fd=fd;
            }
        }
        //建立模型连接可读文件，
        int n=select(max_fd+1,&readfds,NULL,NULL,0);
        if(n<0)
        {
            perror("select");
            _exit(EXIT_FAILURE);
        }
        
        //遍历发现改变的
        for(int fd=0;fd<=max_fd;fd++)
        {
            if(FD_ISSET(fd,&readfds))
            {
                if(fd == server)
                {
                    int newsock = accept(server,NULL,NULL);
                    FD_SET(newsock,&fds);
                    printf("has a new link\n");
                }
                else{
                    memset(buf,0,sizeof(buf));
                    if(read(fd,buf,1024)>0)
                    {
                        printf("%s\n",buf);
                        write(fd,"recved\n",8);
                    }
                    else{
                        printf("recv a close\n"); 
                        close(fd);

                        //把已经关闭的从fds和readfds中移除
                        FD_CLR(fd,&fds);
                        FD_CLR(fd,&readfds);
                        
                    }
                }
            }
        }

    }
    
}
