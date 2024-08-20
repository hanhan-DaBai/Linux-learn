#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MIN_FD 3
#define PORT 5001
struct user
{
   char user[10];
   int fd;
   char ip[20];
   struct user *next;
   struct user *pre;
};

struct user *head = NULL;

void add_user(struct user *p)
{
    struct user *temp,*temp1;
    if(head == NULL)
    {
         head = (struct user *)malloc(sizeof(struct user));
         *head = *p;
         head->next = NULL;
         head->pre = NULL;
    }
    else
    {
        temp = head;
        while(temp->next != NULL) temp = temp->next;
        temp1 = temp;
        temp->next = (struct user *)malloc(sizeof(struct user));
        temp = temp->next;
        temp->pre = temp1;
        *temp = *p;
        temp->next = NULL;
    }    
}

void del_user(int fd)
{
   struct user *temp = head;
   while(temp->fd != fd)
   {
       if( temp->next != NULL)
         temp = temp->next;
       else
       {
          printf("查无此人！\n");
          break;
       }
   }
   if(temp->fd == fd)
   {
      if(temp->pre == NULL)
      {
         head = head->next;
         head->pre = NULL;   
      }
      else if(temp->next == NULL)
      {
          temp->pre->next = NULL;
      }
      else
      {
          temp->pre->next = temp->next;
          temp->next->pre = temp->pre; 
      }
       free(temp);
       printf("删除成功！\n");
  }
}


char * check(int fd)
{

   struct user *temp = head;
   if(temp == NULL) return 0;
   while(temp->fd != fd)
   {
       if( temp->next != NULL)
         temp = temp->next;
       else
       {
          return NULL;
       }
   }
   return temp->user;
}


void broadcast(char *msg)
{
   struct user *temp = head;
   while(temp !=NULL)
   {
       write(temp->fd,msg,strlen(msg));
       temp = temp->next;
   }
}




int main()
{
    fd_set readfs,fds;
    char passwd[10] = {};
    char readbuf[1024];
    char broadcastbuf[1500];
    int maxfd = 0;
    //1、创建socket
    int server = socket(AF_INET,SOCK_STREAM,0);
    if(server < 0)
    {
        perror("server create failed!\n");
        _exit(-1);
    }
    //2、绑定
     struct sockaddr_in host_addr;
     host_addr.sin_family = AF_INET;
     host_addr.sin_port = htons(PORT);
     host_addr.sin_addr.s_addr = INADDR_ANY; //"0.0.0.0"
     
     if(bind(server,( struct sockaddr*)&host_addr,sizeof(host_addr)) < 0)
     {
        perror("bind failed!\n");
        _exit(-2);
     }
 
     //监听
     listen(server,50);
     
     FD_SET(server,&fds);
     maxfd = server;

    printf("please set a talk passwd!\n");
    scanf("%s",passwd);

     while(1)
     {
        //把要监听的所有socket 放入readfs
        readfs = fds;
        if(select(maxfd+1,&readfs,NULL,NULL,0) < 0) //一直等
        {
            perror("select err!\n");
            continue;
        }
        //正常
        for(int fd = MIN_FD;fd <= maxfd;fd++)
        {
           if(FD_ISSET(fd,&readfs))
           {
               if(fd == server)
               {
                  //连接请求
                  int newsock = accept(fd,NULL,NULL);
                  if(newsock > MIN_FD)
                  {
                     FD_SET(newsock,&fds);
                     maxfd = maxfd < newsock?newsock:maxfd;
                  }  
                  printf("accept a new link~\n");  
               }
               else
               {
                   memset(readbuf,0,sizeof(readbuf));
                   char *user = check(fd);
                   if(read(fd,readbuf,1024) > 0)
                   {
                       printf("recv:%s\n",readbuf); 
                       //数据
                        if(user == NULL)
                        {
                            if(strstr(readbuf,"login") == NULL)
                               write(fd,"please login first!--->login:username,passwd\n",strlen("please login first!--->login:username,passwd\n"));
                            else
                            {
                               // "login:username,passwd"
                                 char *p = strtok(readbuf,":"); 
                                 if(p == NULL)  goto format_err;
                                 p = strtok(NULL,",");
                                 if(p == NULL) goto format_err;
                                 struct user User;
                                 User.fd = fd;
                                 strcpy(User.user,p);
                                 p = strtok(NULL,",");
                                 if(p == NULL) goto format_err;
                                 if(strcmp(p,passwd) == 0)
                                 {
                                     add_user(&User);
                                     sprintf(broadcastbuf,"welcome %s enter talk room\n",User.user);
                                     broadcast(broadcastbuf);
                                 }
                                 else
                                 {
                                     write(fd,"passwd err!\n",strlen("passwd err!\n"));
                                 }
                                 continue;
                            }   
                          format_err:
                             write(fd,"login format:\"login:username,passwd\"\n",strlen("login format:\"login:username,passwd\"\n"));
                        } 
                        else
                        {
                            sprintf(broadcastbuf,"%s:%s\n",user,readbuf);
                            broadcast(broadcastbuf);
                        }
                   }
                   else
                   {
                        sprintf(broadcastbuf,"%s quit!\n",user);
                        broadcast(broadcastbuf);
                        //断开请求
                        FD_CLR(fd,&fds);
                        close(fd); 
                   }
               }
           }     
        }
     }
}