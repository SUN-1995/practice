#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <iostream>
using namespace std;
pthread_cond_t taxiCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t taxiMutex = PTHREAD_MUTEX_INITIALIZER;
int travelerCound = 0;
void *traveler_arrive(void *name)
{
    cout << "Traveler : " << (char *)name << " now need a taxi!" << endl;
    pthread_mutex_lock(&taxiMutex);
    travelerCound++;
    pthread_cond_wait(&taxiCond, &taxiMutex);
    pthread_mutex_unlock(&taxiMutex);
    cout << "Traveler: " << (char *)name << " now got a taxi!" << endl;
    pthread_exit((void *)0);
}
void *taxi_arrive(void *name)
{
    cout << "Taxi: " << (char *)name << " arrives." << endl;
    while (1)
    {
        pthread_mutex_lock(&taxiMutex);
        if (travelerCound > 0)
        {
            pthread_cond_signal(&taxiCond);
            pthread_mutex_unlock(&taxiMutex);
            break;
        }
        pthread_mutex_unlock(&taxiMutex);
    }
    pthread_exit((void *)0);
}
int main()
{
    pthread_t tid[3];
    int iRet = pthread_create(&tid[0], NULL, taxi_arrive, (void *)(" Jack "));
    if (iRet)
    {
        printf("pthread_create error: iRet=%d\n", iRet);
        return iRet;
    }
    printf("Time passing by.\n");
    sleep(1);
    iRet = pthread_create(&tid[1], NULL, traveler_arrive, (void *)(" Susan "));
    if (iRet)
    {
        printf("pthread_create error: iRet=%d\n", iRet);
        return iRet;
    }
    printf("Time passing by.\n");
    sleep(1);
    iRet = pthread_create(&tid[2], NULL, taxi_arrive, (void *)(" Mike "));
    if (iRet)
    {
        printf("pthread_create error: iRet=%d\n", iRet);
        return iRet;
    }
    printf("Time passing by.\n");
    sleep(1);
    void *retval;
    for (int i = 0; i < 3; ++i)
    {
        iRet = pthread_join(tid[i], &retval);
        printf("Time passing by.\n");
        if (iRet)
        {
            printf("pthread_join error: iRet=%d\n", iRet);
            return iRet;
        }
        printf("retval=%ld\n", (long)retval);
    }
    return 0;
}