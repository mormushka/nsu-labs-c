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

void do_case(rotation_function first_rotate, rotation_function second_rotate, rbt *check_node,
             rbt **node, rbt *parent, rbt *grand_parent, rbt *uncle, rbt **root, rbt **family, int *len)
{
    if (color(uncle) == RED)
    {
        parent->color = BLACK;
        uncle->color = BLACK;
        grand_parent->color = RED;
        *node = grand_parent;
        *len -= 2;
    }
    else
    {
        if (*node == check_node)
        {
            first_rotate(root, parent, family, *len - 1);
            family[*len - 1] = *node;
        }
        family[*len - 1]->color = BLACK;
        family[*len - 2]->color = RED;
        second_rotate(root, grand_parent, family, *len - 2);
    }
}

void balance(rbt **root, rbt *node, rbt **family, int len)
{
    while ((len != 0) && (family[len - 1]->color == RED))
    {
        rbt *parent = family[len - 1];
        rbt *grand_parent = family[len - 2];
        if (parent == grand_parent->left)
        {
            rbt *uncle = grand_parent->right;
            do_case(rotate_left, rotate_right, parent->right, &node, parent, grand_parent, uncle, root, family, &len);
        }
        else
        {
            rbt *uncle = grand_parent->left;
            do_case(rotate_right, rotate_left, parent->left, &node, parent, grand_parent, uncle, root, family, &len);
        }
    }
    (*root)->color = BLACK;
}

void insert(int data, rbt **root, rbt **family, t_memory *memory)
{
    rbt *new_node = create_leaf(data, memory);
    rbt *p = *root;

    int len = 0;
    while (p != NULL)
    {
        family[len] = p;
        len++;
        if (new_node->data < p->data)
        {
            p = p->left;
        }
        else
        {
            p = p->right;
        }
    }

    if (len == 0)
    {
        *root = new_node;
    }
    else if (new_node->data < family[len - 1]->data)
    {
        family[len - 1]->left = new_node;
    }
    else
    {
        family[len - 1]->right = new_node;
    }

    balance(root, new_node, family, len);
}

int input_tree(rbt **root, int number_nodes, t_memory *memory)
{
    *root = NULL;
    rbt *family[log_2(number_nodes + 1) * 2];

    for (int i = 0; i < number_nodes; ++i)
    {
        int value = 0;
        if (scanf("%d", &value) < 1)
        {
            DEBUG_PRINT("Input error");
            return EIO;
        }

        insert(value, root, family, memory);
    }

    return EXIT_SUCCESS;
}

int height(rbt *t, int h)
{
    return t ? height(t->right, (t->color == BLACK) + h) : h;
}
