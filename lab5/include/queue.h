#ifndef QUEUE_H
#define QUEUE_H

#include "tree.h"

typedef struct tqueue_node
{
    ttree *node;
    struct tqueue_node *next;
} tqueue_node;

typedef struct tqueue
{
    tqueue_node *head;
} tqueue;

tqueue *create_queue(void);
int is_empty(tqueue *queue);
int push(ttree *node, tqueue *queue);
tqueue *build_priority_queue(size_t *hist, int len);
ttree *pop_min(tqueue *queue);

#endif
