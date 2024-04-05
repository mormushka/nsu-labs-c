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
    char height;
    char color;
    struct rbt *parent;
    struct rbt *left;
    struct rbt *right;
} rbt;

rbt *allocate_node(t_memory *memory);
rbt *create_leaf(int value, t_memory *memory, rbt *parent);
char height(rbt *t);
int balance_factor(rbt *t);
void fix_height(rbt *t);
rbt *rotate_right(rbt *t);
rbt *rotate_left(rbt *t);
rbt *balance(rbt *t);
void insert(int value, rbt *parent, rbt **t, t_memory *memory);
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

rbt *create_leaf(int value, t_memory *memory, rbt *parent)
{
    rbt *new = allocate_node(memory);

    new->value = value;
    new->height = 1;
    new->left = NULL;
    new->right = NULL;
    new->parent = parent;

    return new;
}

char height(rbt *t)
{
    return t ? t->height : 1;
}

char color(rbt *t)
{
    return t ? t->color : BLACK;
}

int balance_factor(rbt *t) ///
{
    return height(t->right) - height(t->left);
}

void fix_height(rbt *t)
{
    char hl = height(t->left);
    char hr = height(t->right);
    if (t->color == RED) 
    {
        t->height = (hl > hr ? hl : hr);
    }
    else 
    {
        t->height = (hl > hr ? hl : hr) + 1;
    }
}

rbt *rotate_left(rbt *t)
{
    rbt *buffer = t->right;
    t->right = buffer->left;
    buffer->left = t;

    buffer->color = t->color;
    t->color = RED;

    fix_height(t);
    fix_height(buffer);
    return buffer;
}

rbt *rotate_right(rbt *t)
{
    rbt *buffer = t->left;
    t->left = buffer->right;
    buffer->right = t;

    buffer->color = t->color;
    t->color = RED;

    fix_height(t);
    fix_height(buffer);
    return buffer;
}

void swap_color(rbt *t)
{
    t->color = RED;
    t->left->color = BLACK;
    t->right->color = BLACK;

    if (!t->parent) 
    {
        t->color = BLACK;
    }
    else 
    {
        t->color = RED;
    }

    fix_height(t->right);
    fix_height(t->left);
    fix_height(t);
}

rbt *balance(rbt *t)
{
    if (!t->parent) 
    {
        t->color = BLACK;
    }
    else 
    {
        t->color = RED;
    }

    fix_height(t);

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

/*rbt *balance1(rbt *t)
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
}*/

void insert(int value, rbt *parent, rbt **t, t_memory *memory)
{
    if (!(*t))
    {
        *t = create_leaf(value, memory, parent);
    }
    else if (value < (*t)->value)
    {
        insert(value, *t, &(*t)->left, memory);
    }
    else
    {
        insert(value, *t, &(*t)->right, memory);
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

        insert(value, NULL, &tree, memory);
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