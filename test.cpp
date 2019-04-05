#include <unistd.h>
#include <pthread.h>
#include<bits/stdc++.h>
using namespace std;
void someFuncMayExit()
{
    exit(1);
}
class GlobalObject
{
  public:
    void doit()
    {
        pthread_mutex_lock(&mutex);
        someFuncMayExit();
        pthread_mutex_unlock(&mutex);
    }
    ~GlobalObject()
    {
        printf("GlobalObject:~GlobalObject\n");
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
        printf("GlobalObject:~GlobalObject cleanning\n");
    }

  private:
    pthread_mutex_t mutex;
};
GlobalObject g_obj;
int main(int argvc,char *argv[])
{
    //g_obj.doit();
    cout<<sizeof(int64_t);
    return 0;
}