#include<stdio.h>
#include<pthread.h>

struct arg_type
{
    int a;
    char b[100];
};

void *say_hello(void *args)
{
    arg_type arg_temp=*(arg_type*)args;
    printf("hello from thread,thread id in pthread=%lu,arg_temp.a=%d,arg_temp.b=%s\n",pthread_self(),arg_temp.a,arg_temp.b);
    pthread_exit((void*)1);
}

int main()
{
    pthread_t tid;
    arg_type arg_temp={10,"I am number one."};
    int iRet=pthread_create(&tid,NULL,say_hello,&arg_temp);
    if(iRet)
    {
        printf("pthread_create error: iRet=%d\n",iRet);
        return iRet;
    }
    printf("thread id in process=%lu\n",tid);
    void *retval;
    iRet=pthread_join(tid,&retval);
    if(iRet)
    {
        printf("pthread_join error: iRet=%d\n",iRet);
        return iRet;
    }
    printf("retval=%ld\n",(long)retval);
    return 0;
}