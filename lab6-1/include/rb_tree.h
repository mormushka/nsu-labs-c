#ifndef RB_TREE_H
#define RB_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "m_math.h"

enum colors
{
    BLACK,
    RED
};

typedef struct rbt
{
    int data;
    enum colors color;
    struct rbt *left;
    struct rbt *right;

} rbt;

rbt *allocate_node(t_memory *memory);
rbt *create_leaf(int data, t_memory *memory);
void rotate_left(rbt **root, rbt *x, rbt **family, int len);
void rotate_right(rbt **root, rbt *x, rbt **family, int len);
char color(rbt *t);
void balance(rbt **root, rbt *node, rbt **family, int len);
void insert(int data, rbt **root, rbt **family, t_memory *memory);
rbt *input_tree(int tree_size, t_memory *memory);
int height(rbt *t, int h);

#endif
