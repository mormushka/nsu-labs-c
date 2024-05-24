#ifndef TREE_H
#define TREE_H

#include "constants.h"
#include "bit_stream.h"
#include <stdlib.h>

typedef struct tree_node
{
    unsigned char symbol;
    int freq;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

tree_node *create_tree_node(const unsigned char symbol, const int freq, tree_node *left, tree_node *right);
int is_leaf(tree_node *node);
tree_node *create_tree(int *frequencies);
void pack_tree(tree_node *root, struct bit_stream *stream);
void destroy_tree(tree_node *root);
tree_node *unpack_tree(struct bit_stream *stream);
error_code unpack(tree_node *root, bit_stream *stream, unsigned char *c);

#endif
