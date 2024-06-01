#include "queue.h"
#include "debug_macros.h"
#include <errno.h>
#include <stdlib.h>

tqueue *create_queue()
{
    tqueue *queue = malloc(sizeof(*queue));
    if (!queue)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    queue->head = NULL;
    return queue;
}

static tqueue_node *create_queue_node(ttree *node)
{
    tqueue_node *queue_node = malloc(sizeof(struct tqueue_node));
    if (!queue_node)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    queue_node->node = node;
    queue_node->next = NULL;
    return queue_node;
}

int is_empty(tqueue *queue)
{
    return queue->head == NULL;
}

int push(ttree *node, tqueue *queue)
{
    tqueue_node *new_node = create_queue_node(node);
    if (!new_node)
    {
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }
    if (is_empty(queue) || (node->freq <= queue->head->node->freq))
    {
        new_node->next = queue->head;
        queue->head = new_node;
        return EXIT_SUCCESS;
    }

    tqueue_node *curr_node = queue->head;
    while (curr_node->next != NULL)
    {
        if (node->freq <= curr_node->next->node->freq)
        {
            break;
        }

        curr_node = curr_node->next;
    }

    new_node->next = curr_node->next;
    curr_node->next = new_node;
    return EXIT_SUCCESS;
}

tqueue *build_priority_queue(size_t *hist, int len)
{
    tqueue *priority_queue = create_queue();
    if (!priority_queue)
    {
        DEBUG_PRINT("");
        return NULL;
    }
    for (int i = 0; i < len; i++)
    {
        if (hist[i] != 0)
        {
            ttree *node = create_tree_node(i, hist[i], NULL, NULL);
            if (!node)
            {
                free(priority_queue);
                DEBUG_PRINT("");
                return NULL;
            }
            if (push(node, priority_queue))
            {
                free(priority_queue);
                DEBUG_PRINT("");
                return NULL;
            }
        }
    }
    return priority_queue;
}

ttree *pop_min(tqueue *queue)
{
    tqueue_node *temp = queue->head;
    queue->head = queue->head->next;
    ttree *node = temp->node;
    free(temp);
    return node;
}