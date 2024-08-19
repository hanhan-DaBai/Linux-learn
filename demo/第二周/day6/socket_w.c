//通过socket套接字实现多进程tcp通信
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
int sockfd;
void *func(void *th);
void handle_signal(int sig);

int main()
{
    int newsock;
    struct sockaddr_in sock_add;
    struct sockaddr address;
    int addlen =sizeof(address);
    pthread_t client;

    //代码完成之后发现，服务端无法关闭，用ctrl+c后，端口还会占用一段时间
    //添加处理函数
    signal(SIGINT,handle_signal);
    //创建套接字
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket");
        _exit(EXIT_FAILURE);
    }
    //配置ip与端口号
    sock_add.sin_family=AF_INET;
    sock_add.sin_port=htons(7080);
    sock_add.sin_addr.s_addr=INADDR_ANY;
    //绑定
    if(bind(sockfd,(struct sockaddr *)&sock_add,sizeof(sock_add))<0)
    {
        perror("bind");
        _exit(EXIT_FAILURE);
    }
    //监听
    if(listen(sockfd,3)<0)
    {
        perror("listen");
        _exit(EXIT_FAILURE);
    }
    //实现多进程通信
    while(1)
    {
        //接收连接请求
        if((newsock=accept(sockfd,&address,&addlen))<0)
        {
            perror("accept");
            _exit(EXIT_FAILURE);
        }
        printf("accept a new\n");
        //创建线程
        pthread_create(&client,NULL,&func,&newsock);
    }
}

//既然创建线程那就必须要有线程函数了
void *func(void *th)
{
    pthread_t thread_id = pthread_self();
    //接收传过来的socket标识符
    int sock=*(int*)th;
    //创建数组接收
    char buf[100];

    while(1)
    {
        //把数组清零
        memset(buf,0,sizeof(buf));
        //读通过tcp传的信息到数组
        if((read(sock,buf,100))<=0)
        {
            perror("close process");  
            break;       
        }
        //当收到quit时关闭线程
        if(strstr(buf,"quit") != NULL) break;
        printf("%s\n",buf);
        //回复ok表示已接收
        write(sock,"ok",5);
        //实现区分不同客户端（进程）
        if(strstr(buf,"sendmode") != NULL) 
        {
           while (1)
           {
                memset(buf,0,sizeof(buf));
                printf("Thread ID: %lu received: %s\n", thread_id, buf);
                scanf("%s",buf);
                if(strstr(buf,"quit")!=NULL)
                {
                    break;
                }

                write(sock,buf,sizeof(buf)); 
                
                write(sock,"\n",2);
           }
        }

    }
    //关闭
    close(sock);

}

void handle_signal(int sig)
{
    if(sig==SIGINT)
    {
        printf("\nServer shutting down...\n");
        close(sockfd); // 关闭服务器套接字
        exit(0);
    }
}