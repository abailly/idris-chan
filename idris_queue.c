// A bare-bone blocking concurrent queue implementation
// from  https://github.com/majek/dump/blob/master/msqueue/queue_lock_mutex.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "idris_queue.h"

/* A cell in the queue */
struct queue_head {
  /* pointer to next cell in queue
     is NULL when no successor */
  struct queue_head *next;
  /* payload of this queue cell */
  void *data;
};

struct queue_root {
    struct queue_head *in_queue;
    struct queue_head *out_queue;
    pthread_mutex_t lock;
};


#ifndef _cas
# define _cas(ptr, oldval, newval) \
         __sync_bool_compare_and_swap(ptr, oldval, newval)
#endif

struct queue_root *init_queue()
{
    struct queue_root *root = malloc(sizeof(struct queue_root));

    pthread_mutex_init(&root->lock, NULL);

    root->in_queue = NULL;
    root->out_queue = NULL;
    return root;
}


void queue_put(void *data,
               struct queue_root *root)
{
  struct queue_head* new = malloc(sizeof(struct queue_head));
  new->data=data;
  while (1) {
    struct queue_head *in_queue = root->in_queue;
    new->next = in_queue;
    if (_cas(&root->in_queue, in_queue, new)) {
      break;
    }
  }
}

void *queue_get(struct queue_root *root)
{
    pthread_mutex_lock(&root->lock);

    if (!root->out_queue) {
        while (1) {
            struct queue_head *head = root->in_queue;
            if (!head) {
                break;
            }
            if (_cas(&root->in_queue, head, NULL)) {
                // Reverse the order
                while (head) {
                    struct queue_head *next = head->next;
                    head->next = root->out_queue;
                    root->out_queue = head;
                    head = next;
                }
                break;
            }
        }
    }

    struct queue_head *head = root->out_queue;
    if (head) {
        root->out_queue = head->next;
    }
    pthread_mutex_unlock(&root->lock);
    void *res= head->data;
    free(head);
    return res;
}
