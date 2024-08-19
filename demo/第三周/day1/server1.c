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
#define PORT 8080
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

    struct sockaddr_in host_addr,colient_addr;
    socklen_t len =sizeof(colient_addr);
    
    signal(SIGINT,handle_signal);

    server=socket(AF_INET,SOCK_DGRAM ,0);
    if(server<0)
    {
        perror("socket");
        _exit(EXIT_FAILURE);
    }

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
       
        char rxbuf[100];
        memset(rxbuf,0,sizeof(rxbuf));
        recvfrom(server,rxbuf,sizeof(rxbuf),0,(struct sockaddr*)&colient_addr,&len);
        printf("%s\n",rxbuf);
        sendto(server,"hello\n",7,0,(struct sockaddr*)&colient_addr,len);
        
    }
   close(server);

}