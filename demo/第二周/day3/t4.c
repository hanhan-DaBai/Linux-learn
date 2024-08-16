#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

int main()
{
    int shmid =shmget(100,100,IPC_CREAT|0664);
    int pid =fork();
    if(pid<0)
    {
        perror("pid");
        _exit(-1);
    }
    if(pid>0)
    {
        int sum1=0;
        int *p=(int *)shmat(shmid,NULL,0);
        for(int i=0;i<5;i++)
        {
            scanf("%d",&p[i]);
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
    }else 
    {
        int sum2=0;
        int *p=(int*)shmat(shmid,NULL,0);
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
    

}