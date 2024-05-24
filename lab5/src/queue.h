#ifndef QUEUE_H
#define QUEUE_H

#include "tree.h"
#include "debug_macros.h"
#include <errno.h>
#include <stdlib.h>

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
tqueue *build_priority_queue(int *hist, int len);
ttree *pop_min(tqueue *queue);

#endif
