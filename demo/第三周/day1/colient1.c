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
#include <arpa/inet.h>

#define PORT 8081

int main()
{
    int server;
    struct sockaddr_in host_addr,colient_addr;
    socklen_t len=sizeof(host_addr);
    if((server=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket");
        _exit(EXIT_FAILURE);
    }
    colient_addr.sin_family=AF_INET;
    colient_addr.sin_port=htons(PORT);
    colient_addr.sin_addr.s_addr=INADDR_ANY;
    bind(server,(struct sockaddr*)&colient_addr,sizeof(colient_addr));
    
    host_addr.sin_family=AF_INET;
    host_addr.sin_port=htons(8080);
    host_addr.sin_addr.s_addr=inet_addr("192.168.137.138");

    while (1)
    { 
        char rxbuf[100],txbuf[100];


        
        memset(txbuf,0,sizeof(txbuf));
        scanf("%s",txbuf);
        sendto(server,txbuf,sizeof(txbuf),0,(struct sockaddr*)&host_addr,len);
        

        memset(rxbuf,0,sizeof(rxbuf));
        recvfrom(server,rxbuf,128,0,(struct sockaddr*)&host_addr,&len);
        printf("%s",rxbuf);
        

    }
    
}