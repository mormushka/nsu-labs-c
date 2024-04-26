#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct b_tree
{
    int n; /*фактическое количество ключей*/
    int *key;
    struct b_tree **child;
} b_tree;

/*бинарный поиск (если значение меньше всех 0, если больше всех n)*/
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

void move_tail(b_tree *dest, int i, b_tree *src, int j)
{
    int t = dest->n - i;
    memmove(dest->key + i, src->key + j, t * sizeof(int));
    memmove(dest->child + i, src->child + j, (t + 1) * sizeof(b_tree *));
}

void add_key(b_tree *bt, int k)
{
    int c = get_child_idx(bt, k);
    bt->n += 1;
    move_tail(bt, c + 1, bt, c);
    bt->key[c] = k;
}

b_tree *create(int t)
{
    b_tree *tmp = calloc(1, sizeof(b_tree));
    if (tmp == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return NULL;
    }
    tmp->key = calloc(2 * t - 1, sizeof(int));
    tmp->child = calloc(2 * t, sizeof(b_tree *));
    if ((tmp->key == NULL) || (tmp->child == NULL))
    {
        free(tmp->key);
        free(tmp->child);
        free(tmp);
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return NULL;
    }
    return tmp;
}

void split(int c, b_tree *p, int t)
{
    b_tree *fc = p->child[c];
    b_tree *nc = create(t);

    nc->n = t - 1;
    fc->n = t - 1;

    move_tail(nc, 0, fc, t);

    p->n += 1;

    move_tail(p, c + 1, p, c);

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
    {
        add_key(bt, k);
    }
    else
    {
        int child_idx = get_child_idx(bt, k);
        if (is_full(bt->child[child_idx], t))
        {
            split(child_idx, bt, t);
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
        if (is_full(*bt, t))
        {
            b_tree *new_root = create(t);
            new_root->child[0] = *bt;
            split(0, new_root, t);
            *bt = new_root;
        }
        insert_non_full(*bt, t, k);
    }
}

b_tree *input_tree(int t, int tree_size)
{
    b_tree *bt = NULL;

    for (int i = 0; i < tree_size; ++i)
    {
        int k = 0;
        if (scanf("%d", &k) < 1)
        {
            fprintf(stderr, "Input error %d\n", __LINE__);
        }

        insert(&bt, t, k);
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
    for (int i = 0; i <= bt->n; ++i)
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