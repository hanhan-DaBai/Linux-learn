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
#include <arpa/inet.h>

//宏定义服务端IP与客户端端口（客户端端口不能与服务端相同）
#define PORT 8081
#define IP "192.168.3.255"

int server;
int main()
{
    //1.创建套接字
    server=socket(AF_INET,SOCK_DGRAM,0);
    if(server<0)
    {
        perror("socket");
        _exit(EXIT_FAILURE);
    }

    //2.设置为广播
    int optval = 1;
    if(setsockopt(server,SOL_SOCKET,SO_BROADCAST,&optval,sizeof(optval))<0)
    {
        perror("setsockopt");
        _exit(EXIT_FAILURE);
    }
    
    //绑定客户端地址（也可以不绑定）
    struct sockaddr_in colient_addr,host_addr;
    colient_addr.sin_family=AF_INET;
    colient_addr.sin_port=htons(PORT);
    colient_addr.sin_addr.s_addr=inet_addr("192.168.137.138");
    bind(server,(struct sockaddr*)&colient_addr,sizeof(colient_addr));

    //设置服务端地址与端口（需要直到和谁通信）
    host_addr.sin_family=AF_INET;
    host_addr.sin_port=htons(8080);
    host_addr.sin_addr.s_addr=inet_addr(IP);
    socklen_t len =sizeof(host_addr);
    while (1)
    { 
        char rxbuf[100],txbuf[100];
        //清空输出数组，并输入新的信息
        memset(txbuf,0,sizeof(txbuf));
        printf("请输入你要发送的信息：");
        scanf("%s",txbuf);
        //发送信息给服务端
        if(sendto(server,txbuf,sizeof(txbuf),0,(struct sockaddr*)&host_addr,len)<0)
        {
            perror("sendto");
            _exit(EXIT_FAILURE);
        }
        
        //清空输入数组，将接收的写入其中然后打印
        memset(rxbuf,0,sizeof(rxbuf));
        if(recvfrom(server,rxbuf,128,0,(struct sockaddr*)&host_addr,&len)<0)
        {
            perror("recvfrom");
            _exit(EXIT_FAILURE);
        }
        printf("%s",rxbuf);
        
    }

}