#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int sock;
    char txbuf[100],rxbuf[100];
    struct sockaddr_in serv_addr;
    if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket_fd");
        _exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7080);

    if(inet_pton(AF_INET,"192.168.137.138",&serv_addr.sin_addr) !=1)
    {
        perror("inet_pton");
        _exit(EXIT_FAILURE);
    }
    if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        perror("CONNECT");
        _exit(EXIT_FAILURE);
    }
    while(1)
    {
        memset(txbuf,0,sizeof(txbuf));
        scanf("%s",txbuf);
        send(sock, txbuf, strlen(txbuf), 0);
        printf("Hello message sent\n");
        if(strstr(txbuf,"quit")!=NULL)
        break;
        if(read(sock,rxbuf,100))
        {
            printf("%s\n",rxbuf);
        }
    }
    close(sock);
}