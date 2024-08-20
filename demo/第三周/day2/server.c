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

#define PORT 8080
struct user
{
    char user[10];
    int fd;
    char ip[20];
    struct user *next; 
    struct user *pre;
}
struct user *head=NULL;

void add_user(struct user *p)
{
    struct user *temp,*temp1;
    if(head==NULL)
    {
       head=(struct user *)malloc(sizeof(struct user));   
       *head=*p;
       head->next=NULL;
       head->pre=NULL
    }
    else
    {
        temp=head;
        while (temp->next!=NULL)temp=temp->next;
        temp->next=(struct user *)malloc(sizeof(struct user));  
        temp=temp->next;
        *temp=*p;   
    }  
}

void del_user(char*username)
{
    struct user *temp=head;
    while (strcmp(temp->user,username)!=0)
    {
        if(temp->next!=NULL)
        temp=temp->next;
        else{
            printf("查无此人\n");
            break;
        }

    }
    if(strcmp(temp->user,username)==0)
    {
       if(temp=head)
       {
            head=head->next;
            head->pre=NULL;
       }
       else if()
    }
    else
    {

    }
    
    
}

int main()
{
    fd_set readfds,fds;
    int max_fd =0;
    char rxbuf[1024];

    int server = socket(AF_INET,SOCK_STREAM,0);
    if(server<0)
    {
        perror("socket");
        _exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in host_addr;
    host_addr.sin_family=AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(server,(struct sockaddr*)&host_addr,sizeof(host_addr))<0)
    {
        perror("bind");
        _exit(EXIT_FAILURE);
    }

    //监听
    if(listen(server,50)<0)
    {
        perror("listen");
        _exit(EXIT_FAILURE);
    }

    FD_ZERO(fds);
    FD_ZERO(readfds);
    FD_SET(server,&fds);
    max_fd=server;
    while (1)
    {
        //把所有要监听的放入readfds
        readfds=fds;
        if(select(max_fd+1,,&readfds,NULL,NULL,0)<0)//一直等
        {
            perror("select");
            _exit(EXIT_FAILURE);
        }
        //
        for(int fd=3;fd<=max_fd;fd++)
        {
            if(FD_ISSET(fd,readfds))
            {
                if(fd==server)
                {   
                    //连接请求
                    int newsock = accept(server,NULL,NULL);
                    if(newsock<0)
                    {
                        perror("accept");
                        _exit(EXIT_FAILURE);
                    }
                    FD_SET(newsock,&fds);
                    printf("has a new link\n");
                    max_fd=max_fd<newsock?newsock:max_fd;
                }
                else{
                    //tcp一包数据最多不超过1.5K
                    if(read(fd,rxbuf,1024)<0)
                    {
                        printf("recv a close\n"); 
                        close(fd);
                        FD_CLR(fd,&fds);
                        FD_CLR(fd,&readfds);
                    }



                }
                   
            }
        }


    }
    
}