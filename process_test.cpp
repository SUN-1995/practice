#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
    pid_t pid;
    pid=fork();
    if(pid<0)
    {
        perror("fail to fork");
        exit(-1);
    }
    else if(pid==0)
    {
        //sleep(2);
        printf("Sub_process PID:%u,PPID:%u\n",getpid(),getppid());
        exit(6);
    }
    else
    {
        int status=-1;
        printf("Parent PID:%u,Sub_process PID:%u\n",getpid(),pid);
        //sleep(2);
        pid=wait(&status);
        if(WIFEXITED(status))
        {
            printf("pid:%u",pid);
            printf("sucess:%d\n",WEXITSTATUS(status));
        }
        
    }
    return 0;
}