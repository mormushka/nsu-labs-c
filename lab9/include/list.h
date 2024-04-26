#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list
{
    int value;
    struct list *next;
} list;

void push(list **l, int value);
void destroy_list(list *l);

#endif
