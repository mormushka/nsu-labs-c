#include "list.h"

void push(list **l, int value)
{
    list *new = malloc(sizeof(list));
    new->value = value;
    new->next = *l;
    *l = new;
}

void pop_left(list **l, int num)
{
    if(!(*l))
    {
        return;
    }

    for (int i = 0; i < num; ++i)
    {
        if ((*l)->next)
        {
            list *tmp = (*l)->next;
            free(*l);
            (*l) = tmp;
        }
        else
        {
            free(*l);
            break;
        }
    }
}

list *get_list(list *l, int index)
{
    for (int i = 0; i < index; ++i)
    {
        if (l->next)
        {
            l = l->next;
        }
        else
        {
            break;
        }
    }
    return l;
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
