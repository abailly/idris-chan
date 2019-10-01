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


int main(int argc, char**argv) {
  test_can_put_and_get_data_in_queue();

  printf("queue tests SUCCESS\n");
  return 0;
}
