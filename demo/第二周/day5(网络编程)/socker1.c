#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <arpa/inet.h>
int main()
{
    char buf[100];
    int socket_fd,new_socket;
    struct sockaddr_in host_addr;
    struct sockaddr address;
    int addlen = sizeof(address);
    
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
        new_socket=accept(socket_fd,&address,&addlen);
        if(read(new_socket, buf, 100)<0)
        {
           perror("read");
            _exit(EXIT_FAILURE); 
        }
        printf("Message from client: %s\n", buf);
    }
    
    close(new_socket);
    close(socket_fd);
    return 0;
}