#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define DEBUG1
#define LIST
#define BIN_SEARCH

#ifndef LIST
typedef struct b_tree
{
    int n;                      // фактическое число ключей
    int *key;         // ключи по возрастанию
    struct b_tree **child; // потомки; либо все NULL, либо все не NULL
} b_tree;
#else
typedef struct b_tree
{
    int n;           // фактическое число ключей
    int *key;        // ключи по возрастанию
    struct b_tree **child; // потомки; либо все NULL, либо все не NULL
} b_tree;
#endif

#ifndef LIST
int get_child_idx(const b_tree *bt, int k)
{
    for (int c = 0; c < bt->n; ++c) // TODO: ускорить
        if (k <= bt->key[c])
            return c;
    return bt->n;
}
#else
#ifdef BIN_SEARCH
int get_child_idx(const b_tree *bt, int k)
{
    int left = 0;
    int right = bt->n - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (bt->key[mid] == k)
            return mid;
        else if (bt->key[mid] < k)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return left;
}
#else
int get_child_idx(const b_tree *bt, int k)
{
    for (int c = 0; c < bt->n; ++c)
        if (k <= bt->key[c])
            return c;
    return bt->n;
}
#endif
#endif

#ifdef DEBUG
bool find(const b_tree *bt, int k)
{
    if (bt == NULL)
        return false;
    int childIdx = get_child_idx(bt, k);
    if (bt->key[childIdx] == k)
        return true;
    return find(bt->child[childIdx], k);
}
#endif

void move_tail(b_tree *src, int i, b_tree *dst, int j)
{
    int t = dst->n - j;
    memmove(dst->key + j, src->key + i, t * sizeof(int));
    memmove(dst->child + j, src->child + i, (t + 1) * sizeof(b_tree *));
}

#ifndef LIST
void add_key(b_tree *bt, int k)
{
    int c = get_child_idx(bt, k);
    bt->n += 1;
    move_tail(bt, c, bt, c + 1);
    bt->key[c] = k;
}
#else
void add_key(b_tree *bt, int k)
{
    int c = get_child_idx(bt, k);
    bt->n += 1;
    memmove(bt->key + c + 1, bt->key + c, (bt->n - c - 1) * sizeof(int));
    bt->key[c] = k;
}
#endif

#ifndef LIST
b_tree *create(int t)
{
    b_tree *tmp = calloc(1, sizeof(b_tree)); // заполним нулями
    tmp->key = calloc(2 * t - 1, sizeof(int));
    tmp->child = calloc(2 * t, sizeof(b_tree));
    return tmp;
}
#else
b_tree *create(int t)
{
    b_tree *tmp = calloc(1, sizeof(b_tree)); // заполним нулями
    tmp->n = 0;
    tmp->key = calloc(2 * t - 1, sizeof(int));
    tmp->child = calloc(2 * t, sizeof(b_tree *));
    return tmp;
}
#endif

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
    return bt->child[0] == NULL; // все NULL или все не NULL
}

#ifndef LIST
bool is_full(b_tree *bt, int t)
{
    return bt->n == 2 * t - 1;
}
#else
bool is_full(b_tree *bt, int t)
{
    return bt->n == 2 * t - 1;
}
#endif

#ifndef LIST
void insert_non_full(b_tree *bt, int t, int k)
{
    if (is_leaf(bt))
        add_key(bt, k);
    else
    {
        int childIdx = get_child_idx(bt, k);
        if (is_full(bt->child[childIdx], t))
            split(childIdx, bt, t);
        insert_non_full(bt->child[get_child_idx(bt, k)], t, k);
    }
}
#else
void insert_non_full(b_tree *bt, int t, int k)
{
    if (is_leaf(bt))
        add_key(bt, k);
    else
    {
        int childIdx = get_child_idx(bt, k);
        if (is_full(bt->child[childIdx], t))
            split(childIdx, bt, t);
        insert_non_full(bt->child[get_child_idx(bt, k)], t, k);
    }
}
#endif

#ifndef LIST
void insert(b_tree **bt, int t, int k)
{
    b_tree *nonFull = create(t);
    nonFull->child[0] = *bt;
    insert_non_full(nonFull, t, k);
    if (nonFull->n > 0)
        *bt = nonFull;
}
#else
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
#endif

#ifdef DEBUG
void print_b_tree(b_tree *bt)
{
    if (is_leaf(bt))
    {
        printf("[ ");
        for (int c = 0; c < bt->n; ++c)
            printf("%d ", bt->key[c]);
        printf("]");
        return;
    }
    printf("[ ");
    for (int c = 0; c < bt->n; ++c)
    {
        print_b_tree(bt->child[c]);
        printf(", %d, ", bt->key[c]);
    }
    print_b_tree(bt->child[bt->n]);
    printf("]");
}
#endif

#ifndef DEBUG
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
        return 0; // Пустое дерево

    if (is_leaf(bt))
        return 1; // Листья имеют высоту 1

    int max_child_height = -1;
    for (int i = 0; i <= bt->n; ++i)
    {
        int child_height = height(bt->child[i]);
        if (child_height > max_child_height)
            max_child_height = child_height;
    }

    return 1 + max_child_height;
}

void destroy_b_tree(b_tree *bt)
{
    if (bt == NULL)
        return;

    for (int i = 0; i < bt->n + 1; ++i)
        destroy_b_tree(bt->child[i]);

    free(bt->key);
    free(bt->child);
    free(bt);
}
#endif

int main()
{
    int t = 2;
#ifndef DEBUG
    int count = 0;
    if (scanf("%d %d", &t, &count) < 2)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    b_tree *bt= input_tree(t, count);

    printf("%d", height(bt));

    destroy_b_tree(bt);
#endif

#ifdef DEBUG
    b_tree *bt = NULL;

    insert(&bt, t, 1);
    insert(&bt, t, 2);
    insert(&bt, t, 3);
    insert(&bt, t, 4);
    insert(&bt, t, 5);
    insert(&bt, t, 6);
    insert(&bt, t, 7);
    insert(&bt, t, 8);
    insert(&bt, t, 9);
    print_b_tree(bt);
#endif

    return 0;
}