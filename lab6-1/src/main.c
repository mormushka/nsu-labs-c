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
    struct rbt *left;
    struct rbt *right;
} rbt;

rbt *allocate_node(t_memory *memory);
rbt *create_leaf(int value, t_memory *memory);
int balance_factor(rbt *t);
rbt *rotate_right(rbt *t);
rbt *rotate_left(rbt *t);
rbt *balance(rbt *t);
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
    new->left = NULL;
    new->right = NULL;

    return new;
}

char color(rbt *t)
{
    return t ? t->color : BLACK;
}

void swap_color(rbt *t)
{
    t->color = RED;
    t->left->color = BLACK;
    t->right->color = BLACK;
}

rbt *rotate_left(rbt *t)
{
    rbt *buffer = t->right;
    t->right = buffer->left;
    buffer->left = t;

    buffer->color = t->color;
    t->color = RED;

    return buffer;
}

rbt *rotate_right(rbt *t)
{
    rbt *buffer = t->left;
    t->left = buffer->right;
    buffer->right = t;

    buffer->color = t->color;
    t->color = RED;

    if ((color(buffer->left) == RED) && (color(buffer->right) == RED))
    {
        swap_color(buffer);
    }

    return buffer;
}

rbt *balance(rbt *t)
{
    if ((color(t->right) == RED) && (color(t->left) == BLACK))
    {
        return rotate_left(t);
    }
    if ((color(t->left) == RED) && (color(t->left->left) == RED)) ///
    {
        return rotate_right(t);
    }
    if ((color(t->left) == RED) && (color(t->right) == RED))
    {
        swap_color(t);
    }
    
    return t;
}

void insert(int value, rbt **t, t_memory *memory)
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
    if (tree)
    {
        tree->color = BLACK;
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