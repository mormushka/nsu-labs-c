#include <stdio.h>
#include <stdlib.h>

#define BLACK 0
#define RED 1

typedef struct t_memory
{
    int count;
    int index;
    void *buffer;
} t_memory;

t_memory create_memory(int count, int size);
void destroy_memory(t_memory *memory);

typedef struct rbt
{
    int value;
    char color;
    struct rbt *parent;
    struct rbt *left;
    struct rbt *right;
} rbt;

rbt *allocate_node(t_memory *memory);
rbt *create_leaf(int value, t_memory *memory);
void insert(int value, rbt **t, t_memory *memory);
rbt *input_tree(int tree_size, t_memory *memory);

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

    rbt *tree = (rbt *)((char *)memory->buffer + memory->index * sizeof(rbt));
    ++memory->index;
    return tree;
}

rbt *create_leaf(int value, t_memory *memory)
{
    rbt *new = allocate_node(memory);

    new->value = value;
    new->color = RED;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;

    return new;
}

char color(rbt *t)
{
    return t ? t->color : BLACK;
}

void rotate_left(rbt **tree, rbt *x)
{
    rbt *buffer = x->right;
    x->right = buffer->left;
    if (buffer->left != NULL)
    {
        buffer->left->parent = x;
    }

    buffer->parent = x->parent;
    if (x->parent == NULL)
    {
        *tree = buffer;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = buffer;
    }
    else
    {
        x->parent->right = buffer;
    }

    buffer->left = x;
    x->parent = buffer;
}

void rotate_right(rbt **tree, rbt *x)
{
    rbt *buffer = x->left;
    x->left = buffer->right;
    if (buffer->right != NULL)
    {
        buffer->right->parent = x;
    }

    buffer->parent = x->parent;
    if (x->parent == NULL)
    {
        *tree = buffer;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = buffer;
    }
    else
    {
        x->parent->left = buffer;
    }
    
    buffer->right = x;
    x->parent = buffer;
}

void balance(rbt **tree, rbt *node)
{
    while (node != *tree && node->parent->color == RED)
    {
        if (node->parent == node->parent->parent->left)
        {
            rbt *uncle = node->parent->parent->right;

            if (color(uncle) == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    rotate_left(tree, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotate_right(tree, node->parent->parent);
            }
        }
        else
        {
            rbt *uncle = node->parent->parent->left;

            if (color(uncle) == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rotate_right(tree, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotate_left(tree, node->parent->parent);
            }
        }
    }

    (*tree)->color = BLACK;
}

void insert(int value, rbt **tree, t_memory *memory)
{
    rbt *new_node = create_leaf(value, memory);
    rbt *current_node = *tree;
    rbt *current_parent = NULL;
    while (current_node != NULL)
    {
        current_parent = current_node;
        if (value < current_node->value)
        {
            current_node = current_node->left;
        }
        else
        {
            current_node = current_node->right;
        }
    }

    new_node->parent = current_parent;

    if (current_parent == NULL)
    {
        new_node->color = BLACK;
        *tree = new_node;
        return;
    }
    else if (value < current_parent->value)
    {
        current_parent->left = new_node;
    }
    else
    {
        current_parent->right = new_node;
    }

    balance(tree, new_node);
}

rbt *input_tree(int tree_size, t_memory *memory)
{
    rbt *tree = NULL;

    for (int i = 0; i < tree_size; ++i)
    {
        int value = 0;
        if (scanf("%d", &value) <= 0)
        {
            fprintf(stderr, "Input error %d\n", __LINE__);
        }

        insert(value, &tree, memory);
    }

    return tree;
}

int height(rbt *t)
{
    if (!t)
    {
        return 1;
    }

    return (t->color == BLACK) + height(t->right);
}

int main()
{
    //freopen("../in.txt", "r", stdin);

    int count = 0;
    if (scanf("%d", &count) <= 0)
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

    rbt *tree = input_tree(count, &memory);
    if (count == 0)
    {
        printf("0");
    }
    else
    {
        printf("%d", height(tree));
    }

    destroy_memory(&memory);

    return EXIT_SUCCESS;
}