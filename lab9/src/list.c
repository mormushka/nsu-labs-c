#include "list.h"

int push(list **l, int value)
{
    list *new = malloc(sizeof(list));
    if (new == NULL)
    {
        return ENOMEM;
    }
    new->value = value;
    new->next = *l;
    *l = new;
    return EXIT_SUCCESS;
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
