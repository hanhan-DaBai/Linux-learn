#include  <stdio.h>
#include  <unistd.h>
#include  <stdlib.h>
 
int main()
{
    int i=0;
    for(;i<2;i++)
    {
        fork();
        printf("A %d",i);
    }
    exit(0);
}
