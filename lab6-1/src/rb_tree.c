#include "rb_tree.h"

rbt *allocate_node(t_memory *memory)
{
    if (memory->count == memory->index)
    {
        return NULL;
    }

    rbt *root = (rbt *)((char *)memory->buffer + memory->index * sizeof(rbt));
    ++memory->index;
    return root;
}

rbt *create_leaf(int data, t_memory *memory)
{
    rbt *new = allocate_node(memory);

    new->data = data;
    new->color = RED;
    new->left = NULL;
    new->right = NULL;

    return new;
}

void rotate_left(rbt **root, rbt *x, rbt **family, int len)
{
    rbt *y = x->right;
    x->right = y->left;
    y->left = x;
    if (len == 0)
    {
        *root = y;
    }
    else if (y->data >= family[len - 1]->data)
    {
        family[len - 1]->right = y;
    }
    else
    {
        family[len - 1]->left = y;
    }
}

void rotate_right(rbt **root, rbt *x, rbt **family, int len)
{
    rbt *y = x->left;
    x->left = y->right;
    y->right = x;
    if (len == 0)
    {
        *root = y;
    }
    else if (y->data < family[len - 1]->data)
    {
        family[len - 1]->left = y;
    }
    else
    {
        family[len - 1]->right = y;
    }
}

char color(rbt *t)
{
    return t ? t->color : BLACK;
}

void balance(rbt **root, rbt *node, rbt **family, int len)
{
    if ((len != 0) && (family[len - 1]->color == RED))
    {
        rbt *parent = family[len - 1];
        rbt *grand_parent = family[len - 2];
        if (parent == grand_parent->left)
        {
            rbt *uncle = grand_parent->right;

            if (color(uncle) == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand_parent->color = RED;
                balance(root, grand_parent, family, len - 2);
            }
            else
            {
                if (node == parent->right)
                {
                    rotate_left(root, parent, family, len - 1);
                    family[len - 1] = node;
                }
                family[len - 1]->color = BLACK;
                family[len - 2]->color = RED;
                rotate_right(root, grand_parent, family, len - 2);
            }
        }
        else
        {
            rbt *uncle = grand_parent->left;

            if (color(uncle) == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand_parent->color = RED;
                balance(root, grand_parent, family, len - 2);
            }
            else
            {
                if (node == parent->left)
                {
                    rotate_right(root, parent, family, len - 1);
                    family[len - 1] = node;
                }
                family[len - 1]->color = BLACK;
                family[len - 2]->color = RED;
                rotate_left(root, grand_parent, family, len - 2);
            }
        }
    }
    (*root)->color = BLACK;
}

void insert(int data, rbt **root, rbt **family, t_memory *memory)
{
    rbt *newNode = create_leaf(data, memory);
    rbt *new_node = *root;

    int len = 0;
    while (new_node != NULL)
    {
        family[len] = new_node;
        len++;
        if (newNode->data < new_node->data)
        {
            new_node = new_node->left;
        }
        else
        {
            new_node = new_node->right;
        }
    }

    if (len == 0)
    {
        *root = newNode;
    }
    else if (newNode->data < family[len - 1]->data)
    {
        family[len - 1]->left = newNode;
    }
    else
    {
        family[len - 1]->right = newNode;
    }

    balance(root, newNode, family, len);
}

rbt *input_tree(int tree_size, t_memory *memory)
{
    rbt *root = NULL;
    rbt *family[log_2(tree_size) * 2];

    for (int i = 0; i < tree_size; ++i)
    {
        int value = 0;
        if (scanf("%d", &value) <= 0)
        {
            fprintf(stderr, "Input error %d\n", __LINE__);
        }

        insert(value, &root, family, memory);
    }

    return root;
}

int height(rbt *t, int h)
{
    return t ? height(t->right, (t->color == BLACK) + h) : h;
}
