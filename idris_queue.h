#ifndef QUEUE_H
#define QUEUE_H

struct queue_root;


/* Initialises a new empty queue */
struct queue_root *init_queue();

void queue_put(void *data,
               struct queue_root *root);

void *queue_get(struct queue_root *root);

#endif // QUEUE_H
