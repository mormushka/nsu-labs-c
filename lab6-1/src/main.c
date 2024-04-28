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
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;

    return new;
}

char color(rbt *t)
{
    return t ? t->color : BLACK;
}

rbt *rotate_right(rbt *t)
{
    rbt *buffer = t->left;
    t->left = buffer->right;
    buffer->right = t;
    return buffer;
}

rbt *rotate_left(rbt *t)
{
    rbt *buffer = t->right;
    t->right = buffer->left;
    buffer->left = t;
    return buffer;
}

void insert(int value, rbt **tree, t_memory *memory)
{
    rbt* new_node = create_leaf(value, memory);
    rbt *current_node = &tree;
    rbt *current_parent = NULL;
    while (current_node != NULL)
    {
        current_parent = current_node;
        if (value < current_node->value)
        {
            current_parent = current_node->left;
        }
        else
        {
            current_parent = current_node->right;
        }
    }

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