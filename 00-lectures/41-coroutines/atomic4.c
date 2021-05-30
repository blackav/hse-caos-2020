#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>

enum { COUNT = 2 };

typedef _Atomic int gthr_mutex_t;

gthr_mutex_t m1 = 1;
gthr_mutex_t m2 = 1;

void gthr_lock(gthr_mutex_t *pm)
{
  while (atomic_exchange_explicit(pm, 0, memory_order_acquire) == 0) {}
}

void gthr_unlock(gthr_mutex_t *pm)
{
  atomic_fetch_add_explicit(pm, 1, memory_order_release);
}

int var1;
int var2;

void *thr(void *ptr)
{
  for (int i = 0; i < 20000000; ++i) {
    gthr_lock(&m1);
    ++var1;
    gthr_unlock(&m1);
    gthr_lock(&m2);
    ++var2;
    gthr_unlock(&m2);
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
