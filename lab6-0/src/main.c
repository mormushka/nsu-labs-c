#include <stdio.h>
#include <stdlib.h>

typedef struct t_memory
{
    int count;
    int index;
    void *buffer;
} t_memory;

t_memory create_memory(int count, int size);
void destroy_memory(t_memory *memory);

typedef struct tavl
{
    char height;
    int value;
    struct tavl *left;
    struct tavl *right;
} tavl;

tavl *allocate_avl_node(t_memory *memory);
tavl *create_leaf(int value, t_memory *memory);
char height(tavl *t);
int balance_factor(tavl *t);
void fix_height(tavl *t);
tavl *rotate_right(tavl *t);
tavl *rotate_left(tavl *t);
tavl *balance(tavl *t);
void insert(int value, tavl **t, t_memory *memory);
tavl *input_tree(int tree_size, t_memory *memory);

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

tavl *allocate_avl_node(t_memory *memory)
{
    if (memory->count == memory->index)
    {
        return NULL;
    }

    tavl *tree = (tavl *)((char *)memory->buffer + memory->index * sizeof(tavl));
    ++memory->index;
    return tree;
}

tavl *create_leaf(int value, t_memory *memory)
{
    tavl *new = allocate_avl_node(memory);

    new->height = 1;
    new->value = value;
    new->left = NULL;
    new->right = NULL;

    return new;
}

char height(tavl *t)
{
    return t ? t->height : 0;
}

int balance_factor(tavl *t)
{
    return height(t->right) - height(t->left);
}

void fix_height(tavl *t)
{
    char hl = height(t->left);
    char hr = height(t->right);
    t->height = (hl > hr ? hl : hr) + 1;
}

tavl *rotate_right(tavl *t)
{
    tavl *buffer = t->left;
    t->left = buffer->right;
    buffer->right = t;
    fix_height(t);
    fix_height(buffer);
    return buffer;
}

tavl *rotate_left(tavl *t)
{
    tavl *buffer = t->right;
    t->right = buffer->left;
    buffer->left = t;
    fix_height(t);
    fix_height(buffer);
    return buffer;
}

tavl *balance(tavl *t)
{
    fix_height(t);

    if (balance_factor(t) == 2)
    {
        if (balance_factor(t->right) < 0)
        {
            t->right = rotate_right(t->right);
        }
        return rotate_left(t);
    }
    if (balance_factor(t) == -2)
    {
        if (balance_factor(t->left) > 0)
        {
            t->left = rotate_left(t->left);
        }
        return rotate_right(t);
    }

    return t;
}

void insert(int value, tavl **t, t_memory *memory)
{
    if (!(*t))
    {
        *t = create_leaf(value, memory);
    }
    else if (value < (*t)->value)
    {
        insert(value, &(*t)->left, memory);
    }
    else
    {
        insert(value, &(*t)->right, memory);
    }

    *t = balance(*t);
}

tavl *input_tree(int tree_size, t_memory *memory)
{
    tavl *tree = NULL;

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

int main()
{
    int count = 0;
    if (scanf("%d", &count) <= 0)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    t_memory memory = create_memory(count, sizeof(tavl));
    if (memory.buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    tavl *tree = input_tree(count, &memory);
    printf("%d", height(tree));

    destroy_memory(&memory);

    return EXIT_SUCCESS;
}