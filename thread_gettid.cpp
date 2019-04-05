#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}
void *func(void *)
{
  pthread_exit((void *)gettid());
}
int main()
{
  void *tem=0;
  pthread_t t1, t2;
  pthread_create(&t1, NULL, func, NULL);
  pthread_join(t1, &tem);
  printf("%ld\n", (long)tem);

  pthread_create(&t2, NULL, func, NULL);
  pthread_join(t2, &tem);
  printf("%ld\n", (long)tem);
  return 0;
}
