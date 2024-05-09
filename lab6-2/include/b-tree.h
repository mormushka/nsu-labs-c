#ifndef B_TREE_H
#define B_TREE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include "debug-macros.h"

typedef struct b_tree
{   
    int n; /*фактическое количество ключей*/
    int *key;
    struct b_tree **child;
} b_tree;

int get_child_idx(const b_tree *bt, int k);
void move_tail(b_tree *dest, int i, b_tree *src, int j);
void add_key(b_tree *bt, int k);
b_tree *create(int t);
int split(int c, b_tree *p, int t);
bool is_leaf(b_tree *bt);
bool is_full(b_tree *bt, int t);
int insert_non_full(b_tree *bt, int t, int k);
int insert(b_tree **bt, int t, int k);
int input_tree(b_tree **bt, int t, int tree_size);
int height(b_tree *bt);
void destroy_b_tree(b_tree *bt);


#endif
