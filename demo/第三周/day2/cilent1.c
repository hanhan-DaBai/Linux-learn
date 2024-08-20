#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>

void* read_msg(void *arg)
{
    int fd = *(int *)arg;
    char buf[1024];
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);   
    while(1)
    {
        memset(buf,0,sizeof(buf));
       if(read(fd,buf,1024))
       {
          printf("%s\n",buf);
       }
       else{
          close(fd);
          printf("disconnect to server!\n");
          return NULL;
       }
    }
    
}

int  main(int argc,char *argv[])
{
     int status = 0;
     pthread_t  readThId;
     char buf[100];
     char txbuf[1024];
     char rxbuf[1024];
     struct sockaddr_in remote_addr;
     
     if(argc < 3) 
     {
        printf("args num too few!\n");
        _exit(-1);
     } 
     int client = socket(AF_INET,SOCK_STREAM,0);
     if(client < 0)
     {
        perror("client create failed!\n");
        _exit(-2);
     }

     remote_addr.sin_family = AF_INET;
     remote_addr.sin_port = htons(atoi(argv[2]));
     remote_addr.sin_addr.s_addr = inet_addr(argv[1]);

     if(connect(client,(struct sockaddr*)&remote_addr,sizeof(remote_addr))< 0)
     {
        perror("conncet failed!\n");
        _exit(-3);
     }
     
     while(1)
     {
        switch(status)
        {
            case 0:
              printf("please input username and passwd:\n");
              scanf("%s",buf);
              sprintf(txbuf,"login:%s",buf);
              write(client,txbuf,strlen(txbuf));
              memset(rxbuf,0,sizeof(rxbuf));
              if(read(client,rxbuf,1024))
              {
                 printf("%s\n",rxbuf);
                 if(strstr(rxbuf,"welcome") != NULL) status = 1;
              }
            break;

            case 1:
               pthread_create(&readThId,NULL,read_msg,&client);
               while(1)
               {
                  scanf("%s",buf);
                  if(strstr(buf,"quit") != NULL)
                  {
                     goto quit;
                  };
                  write(client,buf,strlen(buf));
               }
            quit:
               pthread_cancel(readThId);
               pthread_join(readThId,NULL);
               close(client);
               printf("quit!\n");
               _exit(0);
            break;
        }
     }
}
