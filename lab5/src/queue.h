#ifndef QUEUE_H
#define QUEUE_H

#include "tree.h"
#include <stdlib.h>

typedef struct queue_node
{
    tree_node *node;
    struct queue_node *next;
} queue_node;

typedef struct queue
{
    queue_node *head;
} queue;

queue *create_queue(void);
int is_empty(queue *queue);
int push(tree_node *node, queue *queue);
queue *build_priority_queue(int *frequencies, int len);
tree_node *pop(queue *queue);

#endif
