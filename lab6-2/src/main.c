#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct b_tree
{
    int n;
    int *key;
    struct b_tree **child;
} b_tree;

int get_child_idx(const b_tree *bt, int k)
{
    int left = 0;
    int right = bt->n - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (bt->key[mid] == k)
        {
            return mid;
        }
        else if (bt->key[mid] < k)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return left;
}

void move_tail(b_tree *src, int i, b_tree *dst, int j)
{
    int t = dst->n - j;
    memmove(dst->key + j, src->key + i, t * sizeof(int));
    memmove(dst->child + j, src->child + i, (t + 1) * sizeof(b_tree *));
}

void add_key(b_tree *bt, int k)
{
    int c = get_child_idx(bt, k);
    bt->n += 1;
    memmove(bt->key + c + 1, bt->key + c, (bt->n - c - 1) * sizeof(int));
    bt->key[c] = k;
}

b_tree *create(int t)
{
    b_tree *tmp = malloc(sizeof(b_tree));
    tmp->key = malloc((2 * t - 1) * sizeof(int));
    tmp->child = malloc(2 * t * sizeof(b_tree *));
    return tmp;
}

void split(int c, b_tree *p, int t)
{
    b_tree *fc = p->child[c];
    b_tree *nc = create(t);
    nc->n = t - 1;
    move_tail(fc, t, nc, 0);
    fc->n = t - 1;
    p->n += 1;
    move_tail(p, c, p, c + 1);
    p->key[c] = fc->key[t - 1];
    p->child[c + 1] = nc;
}

bool is_leaf(b_tree *bt)
{
    return bt->child[0] == NULL;
}

bool is_full(b_tree *bt, int t)
{
    return bt->n == 2 * t - 1;
}

void insert_non_full(b_tree *bt, int t, int k)
{
    if (is_leaf(bt))
        add_key(bt, k);
    else
    {
        int childIdx = get_child_idx(bt, k);
        if (is_full(bt->child[childIdx], t))
        {
            split(childIdx, bt, t);
        }
        insert_non_full(bt->child[get_child_idx(bt, k)], t, k);
    }
}

void insert(b_tree **bt, int t, int k)
{
    if (*bt == NULL)
    {
        *bt = create(t);
        (*bt)->key[0] = k;
        (*bt)->n = 1;
    }
    else
    {
        b_tree *nonFull = *bt;
        if (is_full(nonFull, t))
        {
            b_tree *newRoot = create(t);
            newRoot->child[0] = nonFull;
            split(0, newRoot, t);
            *bt = newRoot;
        }
        insert_non_full(*bt, t, k);
    }
}

b_tree *input_tree(int t, int tree_size)
{
    b_tree *bt = NULL;

    for (int i = 0; i < tree_size; ++i)
    {
        int value = 0;
        if (scanf("%d", &value) <= 0)
        {
            fprintf(stderr, "Input error %d\n", __LINE__);
        }

        insert(&bt, t, value);
    }

    return bt;
}

int height(b_tree *bt)
{
    if (bt == NULL)
    {
        return 0;
    }

    if (is_leaf(bt))
    {
        return 1;
    }

    return 1 + height(bt->child[0]);
}

void destroy_b_tree(b_tree *bt)
{
    if (bt == NULL)
    {
        return;
    }
    for (int i = 0; i < bt->n + 1; ++i)
    {
        destroy_b_tree(bt->child[i]);
    }

    free(bt->key);
    free(bt->child);
    free(bt);
}

int main()
{
    int t = 2;
    int count = 0;
    if (scanf("%d %d", &t, &count) < 2)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    b_tree *bt = input_tree(t, count);

    printf("%d", height(bt));

    destroy_b_tree(bt);

    return EXIT_SUCCESS;
}