#include "list.h"

void push(list **l, int value)
{
    list *new = malloc(sizeof(list));
    new->value = value;
    new->next = *l;
    *l = new;
}

void destroy_list(list *l)
{
    while (l != NULL)
    {
        list *next = l->next;
        free(l);
        l = next;
    }
}
