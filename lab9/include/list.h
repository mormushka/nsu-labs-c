#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <errno.h>

typedef struct list
{
    int value;
    struct list *next;
} list;

int push(list **l, int value);
void destroy_list(list *l);

#endif
