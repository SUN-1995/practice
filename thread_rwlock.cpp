#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
pthread_rwlock_t rwlock;
void *readers(void *arg)
{
    pthread_rwlock_rdlock(&rwlock);
    printf("reader %ld got the lock\n", (long)arg);
    pthread_rwlock_unlock(&rwlock);
    pthread_exit((void *)0);
}
void *writers(void *arg)
{
    pthread_rwlock_wrlock(&rwlock);
    printf("writer %ld got the lock\n", (long)arg);
    pthread_rwlock_unlock(&rwlock);
    pthread_exit((void *)0);
}
int main()
{
    int iRet, i;
    pthread_t writer_id, reader_id;
    pthread_attr_t attr;
    int nreadercount = 1, nwritercount = 1;
    iRet = pthread_rwlock_init(&rwlock, NULL);
    if (iRet)
    {
        fprintf(stderr, "init lock failed\n");
        return iRet;
    }
    pthread_attr_init(&attr);
    //设置线程分离状态，状态设置为PTHREAD_CREATE_DETACHED，表示已分离状态启动线程
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for (i = 0; i < 5; ++i)
    {
        if (i % 3)
        {
            pthread_create(&reader_id, &attr, readers, (void *)nreadercount);
            printf("create reader %d\n", nreadercount++);
        }
        else
        {
            pthread_create(&writer_id, &attr, writers, (void *)nwritercount);
            printf("create writer %d\n", nwritercount++);
        }
    }
    sleep(5);//等待另外线程的执行
    return 0;
}