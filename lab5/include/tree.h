#ifndef TREE_H
#define TREE_H

#include "constants.h"
#include "bit_stream.h"
#include "debug_macros.h"
#include <stdlib.h>

typedef struct ttree
{
    unsigned char symbol;
    unsigned freq;
    struct ttree *left;
    struct ttree *right;
} ttree;

ttree *create_tree_node(const unsigned char symbol, const unsigned freq, ttree *left, ttree *right);
int is_leaf(ttree *node);
ttree *create_tree(unsigned *hist);
int pack_tree(ttree *root, tbit_stream *bit_stream);
ttree *unpack_tree(tbit_stream *bit_stream, int *error_code);
int unpack(ttree *root, tbit_stream *bit_stream, unsigned char *c);
void destroy_tree(ttree *root);

#endif
