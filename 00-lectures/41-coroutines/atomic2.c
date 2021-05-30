#include <pthread.h>
#include <stdio.h>

enum { COUNT = 2 };

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

int var1;
int var2;

void *thr(void *ptr)
{
  for (int i = 0; i < 20000000; ++i) {
    pthread_mutex_lock(&m1);
    ++var1;
    pthread_mutex_unlock(&m1);
    pthread_mutex_lock(&m2);
    ++var2;
    pthread_mutex_unlock(&m2);
  }
  return NULL;
}

int main()
{
  pthread_t ids[COUNT];

  for (int i = 0; i < COUNT; ++i) {
    pthread_create(&ids[i], NULL, thr, NULL);
  }

  for (int i = 0; i < COUNT; ++i) {
    pthread_join(ids[i], NULL);
  }

  printf("%d\n%d\n", var1, var2);
}
