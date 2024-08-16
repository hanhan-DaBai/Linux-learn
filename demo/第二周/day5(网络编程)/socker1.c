#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netinet/ip.h> 


void *func(void *th)
{
    char buf[100];
    int sock=*(int*)th;
    int cnt;
    while(1)
    {
        memset(buf,0,sizeof(buf));
        cnt = read(sock,buf,100);
        if(cnt > 0)
        {
            if(strstr(buf,"quit") != NULL) break;
            printf("recv data:%s\n",buf);
        }
        else if(cnt == 0) break;  //挥手信号，如果对方断开了，这里还执行write函数，会使进程异常退出。
        write(sock,"hello",5);
    }
    close(sock);
}
int main()
{
    char buf[100];
    int socket_fd;
    struct sockaddr_in host_addr;
    struct sockaddr address;
    int addlen = sizeof(address);
    pthread_t cliend_pth;
    
    if((socket_fd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket_fd");
        _exit(EXIT_FAILURE);
    }

    //绑定
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(7080);
    host_addr.sin_addr.s_addr =  INADDR_ANY;
    if(bind(socket_fd,(struct sockaddr *)&host_addr,sizeof(host_addr))<0)
    {
        perror("bind");
        _exit(EXIT_FAILURE);
    }
    if(listen(socket_fd,50)<0)
    {
        perror("listen");
        _exit(EXIT_FAILURE);
    } 
    while (1)
    {
        int new_socket=accept(socket_fd,&address,&addlen);
        printf("accept a new conoection\n");
        pthread_create(&cliend_pth,NULL,func,&new_socket);
        
    }
    
    
    return 0;
}