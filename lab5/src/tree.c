#include "tree.h"
#include "queue.h"
#include "constants.h"
#include "debug_macros.h"
#include <stdlib.h>
#include <errno.h>

ttree *create_tree_node(const unsigned char symbol, const size_t freq, ttree *left, ttree *right)
{
    ttree *node = malloc(sizeof(ttree));
    if (!node)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    node->symbol = symbol;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

int is_leaf(ttree *node)
{
    return !node->left;
}

void destroy_tree(ttree *root)
{
    if (!root)
    {
        return;
    }

    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

int pack_tree(ttree *root, tbit_stream *bit_stream)
{
    if (is_leaf(root))
    {
        if (write_bit(1, bit_stream) || write_byte(root->symbol, bit_stream))
        {
            DEBUG_PRINT("");
            return EIO;
        }
        return EXIT_SUCCESS;
    }
    if (write_bit(0, bit_stream) || pack_tree(root->left, bit_stream) || pack_tree(root->right, bit_stream))
    {
        DEBUG_PRINT("");
        return EIO;
    }
    return EXIT_SUCCESS;
}

ttree *unpack_tree(tbit_stream *bit_stream, int *error_code)
{
    int bit;
    if (read_bit(bit_stream, &bit))
    {
        *error_code = EIO;
        DEBUG_PRINT("");
        return NULL;
    }
    if (bit == 1)
    {
        unsigned char byte;
        if (read_byte(bit_stream, &byte))
        {
            *error_code = EIO;
            DEBUG_PRINT("");
            return NULL;
        }
        ttree *tmp = create_tree_node(byte, 0, NULL, NULL);
        if (!tmp)
        {
            *error_code = ENOMEM;
            DEBUG_PRINT("");
            return NULL;
        }
        return tmp;
    }
    ttree *left = unpack_tree(bit_stream, error_code);
    ttree *right = unpack_tree(bit_stream, error_code);
    ttree *tmp = create_tree_node(0, 0, left, right);
    if (!tmp)
    {
        *error_code = ENOMEM;
        DEBUG_PRINT("");
        return NULL;
    }
    return tmp;
}

ttree *create_tree(size_t *hist)
{
    if (hist == NULL)
    {
        return NULL;
    }

    tqueue *priority_queue = build_priority_queue(hist, ALPHABET_SIZE);
    if (!priority_queue)
    {
        DEBUG_PRINT("");
        return NULL;
    }

#ifndef NDEBUG
    fprintf(stderr, "# PRIOTITY QUEUE:\n");
    tqueue_node *tmp = priority_queue->head;
    while (tmp)
    {
        fprintf(stderr, "%.2x - %zu\n", (int)(tmp->node->symbol), tmp->node->freq);
        tmp = tmp->next;
    }
    fprintf(stderr, "# END PRIOTITY QUEUE\n\n");
#endif

    if (priority_queue->head->next == NULL)
    {
        ttree *root = pop_min(priority_queue);
        free(priority_queue);
        return root;
    }

    for (;;)
    {
        ttree *left = pop_min(priority_queue);
        ttree *right = pop_min(priority_queue);
        ttree *root = create_tree_node(0, left->freq + right->freq, left, right);
        if (!root)
        {
            free(priority_queue);
            DEBUG_PRINT("");
            return NULL;
        }

        if (is_empty(priority_queue))
        {
            free(priority_queue);
            return root;
        }

        if (push(root, priority_queue))
        {
            free(priority_queue);
            DEBUG_PRINT("");
            return NULL;
        }
    }
}

int unpack(ttree *root, tbit_stream *bit_stream, unsigned char *c)
{
    if (is_leaf(root))
    {
        int bit;
        if (read_bit(bit_stream, &bit))
        {
            return EIO;
        }
        *c = root->symbol;
        return EXIT_SUCCESS;
    }

    ttree *curr_node = root;
    while (!is_leaf(curr_node))
    {
        int bit;
        if (read_bit(bit_stream, &bit))
        {
            return EIO;
        }
        if (bit == 0)
        {
            curr_node = curr_node->left;
        }
        else
        {
            curr_node = curr_node->right;
        }
    }

    *c = curr_node->symbol;
    return EXIT_SUCCESS;
}