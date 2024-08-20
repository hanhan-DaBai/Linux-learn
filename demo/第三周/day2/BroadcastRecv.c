#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <signal.h>
//宏定义服务端端口
#define PORT 8080
int server;
//ctrl+C关闭终端释放服务器
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

    struct sockaddr_in host_addr,colient_addr;
    socklen_t len =sizeof(colient_addr);
    //接收CTRL+C信号
    signal(SIGINT,handle_signal);
    //生成套接字
    server=socket(AF_INET,SOCK_DGRAM ,0);
    if(server<0)
    {
        perror("socket");
        _exit(EXIT_FAILURE);
    }
    //2.设置端口复用
    int optval = 1;
    if(setsockopt(server,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval))<0)
    {
        perror("setsockopt");
        _exit(EXIT_FAILURE);
    }

    //绑定服务端地址与端口
    host_addr.sin_family=AF_INET;
    host_addr.sin_port=htons(PORT);
    host_addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(server,(struct sockaddr *)&host_addr,sizeof(host_addr))<0)
    {
        perror("bind");
        _exit(EXIT_FAILURE);
    }
    
    while (1)
    {
        //清空输入数组，将接收的写入其中然后打印
        char rxbuf[100];
        memset(rxbuf,0,sizeof(rxbuf));
        int n = recvfrom(server,rxbuf,sizeof(rxbuf),0,(struct sockaddr*)&colient_addr,&len);
        if(n<0)
        {
            perror("recvfrom");
            _exit(EXIT_FAILURE);
        }
        printf("%s\n",rxbuf);
        //接收到后向客户端发送ok
        if(n>0)
        {
            int m= sendto(server,"ok\n",7,0,(struct sockaddr*)&colient_addr,len);
            if(m<0)
            {
                perror("sendto");
                _exit(EXIT_FAILURE);
            }
        } 
    }
   close(server);

}