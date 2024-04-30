#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG1

typedef struct t_memory
{
    int count;
    int index;
    void *buffer;
} t_memory;

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

t_memory create_memory(int count, int size)
{
    t_memory memory = {count, 0, NULL};

    memory.buffer = malloc(count * size);

    return memory;
}

void destroy_memory(t_memory *memory)
{
    free(memory->buffer);
}

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
    rbt *parent = NULL;
    rbt *grand_parent = NULL;
    if ((len != 0) && (family[len - 1]->color == RED))
    {
        rbt *parent = family[len - 1];
        grand_parent = family[len - 2];
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

int pow_m(int num, int d)
{
    return d == 0 ? 1 : num * pow_m(num, d - 1);
}

int log_2(int n)
{
    for (int i = 0; i < 31; i++)
    {
        if (n < pow_m(2, i))
        {
            return i;
        }
    }
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

int main()
{
#ifdef DEBUG
    freopen("../in.txt", "r", stdin);
#endif

    int count = 0;
    if (scanf("%d", &count) < 1)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    t_memory memory = create_memory(count, sizeof(rbt));
    if (memory.buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    rbt *root = input_tree(count, &memory);
    printf("%d", height(root, root != NULL));

    destroy_memory(&memory);

    return EXIT_SUCCESS;
}