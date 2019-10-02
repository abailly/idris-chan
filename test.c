#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "idris_queue.h"

static char data[] = "this is a test";

void test_can_put_and_get_data_in_queue() {
  struct queue_root* queue = init_queue();
  assert(queue);

  queue_put(data, queue);

  void * res = queue_get(queue);
  assert (res == data);
}

void *writer(void *q) {
  queue_put(data, (struct queue_root*) q);
}

void *reader(void *q) {
  void *res = queue_get((struct queue_root*) q);
  assert (res == data);
}

void test_can_exchange_data_between_threads_through_queue() {
  pthread_t thread1, thread2;
  pthread_attr_t attr1, attr2;

  // shared queue between all threads
  struct queue_root *queue = init_queue();

  int res = pthread_attr_init(&attr1);
  assert(!res);
  res = pthread_attr_init(&attr2);
  assert(!res);
  pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);

  res = pthread_create(&thread1,&attr1,writer,queue);
  assert(!res);

  res = pthread_create(&thread1,&attr1,reader,queue);
  assert(!res);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  pthread_attr_destroy(&attr1);
  pthread_attr_destroy(&attr2);
}

int main(int argc, char**argv) {
  test_can_put_and_get_data_in_queue();
  test_can_exchange_data_between_threads_through_queue();

  printf("queue tests SUCCESS\n");
  return 0;
}
