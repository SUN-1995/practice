#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t mutex_x = PTHREAD_MUTEX_INITIALIZER;
int total_ticket_num = 20;
void *sell_ticket(void *arg)
{
    for (int i = 0; i < 20; ++i)
    {
        pthread_mutex_lock(&mutex_x);
        if (total_ticket_num > 0)
        {
            printf("thread1 sell the %dth ticket\n", 20 - total_ticket_num + 1);
            --total_ticket_num;
        }
        sleep(1);
        pthread_mutex_unlock(&mutex_x);
        sleep(1);
    }
    return 0;
}
void *sell_ticket2(void *arg)
{
    int iRet;
    for (int i = 0; i < 20; ++i)
    {
        iRet = pthread_mutex_trylock(&mutex_x);
        if (iRet == EBUSY)
        {
            printf("sell_ticket2 : the variable is locked by sell_ticketl. \n ");
        }
        else if (iRet == 0)
        {
            if (total_ticket_num > 0)
            {
                printf("thread2 sell the %dth ticket\n", 20 - total_ticket_num + 1);
                --total_ticket_num;
            }
            pthread_mutex_unlock(&mutex_x);
        }
        sleep(1);
    }
    return 0;
}
int main()
{
    int iRet;
    pthread_t tids[2];
    int i = 0;

    iRet = pthread_create(&tids[1], NULL, &sell_ticket, NULL);
    if (iRet)
    {
        printf("pthread_created error, iRet=%d\n", iRet);
        return iRet;
    }
    iRet = pthread_create(&tids[2], NULL, &sell_ticket2, NULL);
    if (iRet)
    {
        printf("pthread_created error, iRet=%d\n", iRet);
        return iRet;
    }

    sleep(30);
    void *retval;

    iRet = pthread_join(tids[1], &retval);

    if (iRet)
    {
        printf("tid=%ld jion error, iRet=%d\n", tids[i], iRet);
        return iRet;
    }
    else
    {
        printf("retval=%ld\n", (long *)retval);
    }

    iRet = pthread_join(tids[2], &retval);
    if (iRet)
    {
        printf("tid=%ld jion error, iRet=%d\n", tids[i], iRet);
        return iRet;
    }
    else
    {
        printf("retval=%ld\n", (long)retval);
    }

    return 0;
}