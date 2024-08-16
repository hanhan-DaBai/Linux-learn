#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <fcntl.h>
int main()
{
    int fd=shm_open("/posoxsm\0",O_CREAT|O_RDWR,0666);
    //创建内存大小
    ftruncate(fd,0x400000);
    //内存映射
    char  *p=mmap(NULL,0x400000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p==MAP_FAILED)
    {
        perror("mmap");
        _exit(EXIT_FAILURE);
    }
    //映射完成后 关闭fd 并不是释放
    close(fd);
    //使用内存映射实现进程间的通信
    int pid=fork();
    if(pid<0)
    {
        perror("fork");
        _exit(EXIT_FAILURE);
    }
    if(pid>0)
    {
        int sum1;
         for(int i=0;i<5;i++)
        {
            scanf("%d",(int*)&p[i]);
            sum1+=p[i];
        }
        p[10]=1;
        while (p[10]!=0)
        {
            usleep(1000);
        }
        printf("sum: %d = %d + %d\n",sum1+p[5],sum1,p[5]);
        wait(NULL);
        shmdt(p);
        _exit(0);
    }
    else
    {
        //父进程
        int sum2=0;
        while (p[10]!=1)
        {
            usleep(1000);
        }
        for (int i = 0; i < 5; i++)
        {
            sum2+=p[i]*p[i];
        }
        p[5]=sum2;
        p[10]=0;
        shmdt(p);
        _exit(0);
        
    }

    //释放映射区
    munmap(p,0x400000);
    //释放共享内存
   shm_unlink(p);
}