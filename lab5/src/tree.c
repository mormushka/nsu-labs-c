#include "queue.h"
#include "tree.h"

tree_node *create_tree_node(const unsigned char symbol, const int freq, tree_node *left, tree_node *right)
{
    tree_node *node = malloc(sizeof(tree_node));
    if (!node)
    {
        return NULL;
    }
    node->symbol = symbol;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

int is_leaf(tree_node *node)
{
    return !node->left;
}

void destroy_tree(tree_node *root)
{
    if (root == NULL)
    {
        return;
    }

    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

void pack_tree(tree_node *root, bit_stream *stream)
{
    if (is_leaf(root))
    {
        write_bit(1, stream);
        write_byte(root->symbol, stream);
        return;
    }
    write_bit(0, stream);
    pack_tree(root->left, stream);
    pack_tree(root->right, stream);
}

tree_node *unpack_tree(bit_stream *stream)
{
    int bit;
    if (read_bit(stream, &bit) == read_error)
    {
        return NULL;
    }
    if (bit == 1)
    {
        unsigned char byte;
        if (read_byte(stream, &byte) == read_error)
        {
            return NULL;
        }
        return create_tree_node(byte, 0, NULL, NULL);
    }
    tree_node *left = unpack_tree(stream);
    tree_node *right = unpack_tree(stream);
    return create_tree_node(0, 0, left, right);
}

tree_node *create_tree(int *frequencies)
{
    queue *priority_queue = build_priority_queue(frequencies, ALPHABET_SIZE);

#ifndef NDEBUG
    fprintf(stderr, "# PRIOTITY QUEUE:\n");
    queue_node *tmp = priority_queue->head;
    while (tmp)
    {
        fprintf(stderr, "%.2x - %d\n", (int)(tmp->node->symbol), tmp->node->freq);
        tmp = tmp->next;
    }
    fprintf(stderr, "# END PRIOTITY QUEUE\n\n");
#endif
    if (priority_queue->head->next == NULL)
    {
        tree_node *root = pop(priority_queue);
        free(priority_queue);
        return root;
    }

    for (;;)
    {
        tree_node *left = pop(priority_queue);
        tree_node *right = pop(priority_queue);
        tree_node *root = create_tree_node(0, left->freq + right->freq, left, right);
        if (!root)
        {
            free(priority_queue);
            return NULL;
        }

        if (is_empty(priority_queue))
        {
            free(priority_queue);
            return root;
        }

        if (!push(root, priority_queue))
        {
            free(priority_queue);
            return NULL;
        }
    }
}

error_code unpack(tree_node *root, bit_stream *stream, unsigned char *c)
{
    tree_node *curr_node = root;
    while (!is_leaf(curr_node))
    {
        int bit;
        if (read_bit(stream, &bit) == read_error)
        {
            return read_error;
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
    return no_error;
}
