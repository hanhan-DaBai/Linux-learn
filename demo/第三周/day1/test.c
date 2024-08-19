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
    server=socket(AF_INET,SOCK_STREAM,0);

    signal(SIGINT,handle_signal);

    if(server<0)
    {
        perror("socket");
        _exit(EXIT_FAILURE);
    }
    //配置ip与端口号
    sock_addr.sin_family=AF_INET;
    sock_addr.sin_port=htons(PROT);
    sock_addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(server,(struct sockaddr *)&sock_addr,sizeof(sock_addr))<0)
    {
        perror("bind");
        _exit(EXIT_FAILURE);
    }
    if(listen(server,100)<0)
    {
        perror("listen");
        _exit(EXIT_FAILURE);
    }

    max_fd =server;
    FD_ZERO(&fds);
    FD_ZERO(&readfds);
    FD_SET(server,&fds);

    while (1)
    {

        for(int fd=0;fd<1024;fd++)
        {
            if(FD_ISSET(fd,&fds))
            {
                FD_SET(fd,&readfds);
                if(fd>max_fd)max_fd=fd;
            }
        }
        select(max_fd+1,&readfds,NULL,NULL,0);
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
                        FD_CLR(fd,&fds);
                        max_fd=0;
                    }
                }
            }
        }

    }
    
}
