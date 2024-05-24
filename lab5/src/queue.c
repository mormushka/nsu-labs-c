#include "queue.h"

queue *create_queue(void)
{
    queue *queue = malloc(sizeof(queue));
    if (!queue)
    {
        return NULL;
    }
    queue->head = NULL;
    return queue;
}

queue_node *create_queue_node(tree_node *node)
{
    queue_node *queue_node = malloc(sizeof(struct queue_node));
    if (!queue_node)
    {
        return NULL;
    }
    queue_node->node = node;
    queue_node->next = NULL;
    return queue_node;
}

int is_empty(queue *queue)
{
    return queue->head == NULL;
}

int push(tree_node *node, queue *queue)
{
    queue_node *new_node = create_queue_node(node);
    if (!new_node)
    {
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }
    if (is_empty(queue) || node->freq <= queue->head->node->freq)
    {
        new_node->next = queue->head;
        queue->head = new_node;
        return EXIT_SUCCESS;
    }

    queue_node *curr_node = queue->head;
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

queue *build_priority_queue(int *frequencies, int len)
{
    queue *priority_queue = create_queue();
    if (!priority_queue)
    {
        return NULL;
    }
    for (int i = 0; i < len; i++)
    {
        if (frequencies[i] != 0)
        {
            tree_node *node = create_tree_node(i, frequencies[i], NULL, NULL);
            if (!node)
            {
                free(priority_queue);
                return NULL;
            }
            if (push(node, priority_queue))
            {
                free(priority_queue);
                return NULL;
            }
        }
    }
    return priority_queue;
}

tree_node *pop(queue *queue)
{
    queue_node *temp = queue->head;
    queue->head = queue->head->next;
    tree_node *node = temp->node;
    free(temp);
    return node;
}
