#include "b-tree.h"

/*бинарный поиск (если значение меньше всех 0, если больше всех n)*/
static int get_child_idx(const b_tree *bt, int k)
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

static b_tree *create(int t)
{
    b_tree *tmp = calloc(1, sizeof(b_tree));
    if (tmp == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    tmp->key = calloc(2 * t - 1, sizeof(int));
    tmp->child = calloc(2 * t, sizeof(b_tree *));
    if ((tmp->key == NULL) || (tmp->child == NULL))
    {
        free(tmp->key);
        free(tmp->child);
        free(tmp);
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    return tmp;
}

static bool is_leaf(b_tree *bt)
{
    return bt->child[0] == NULL;
}

static bool is_full(b_tree *bt, int t)
{
    return bt->n == 2 * t - 1;
}

/*копирование хвоста из src + j в dest + i*/
static void move_tail(b_tree *dest, int i, b_tree *src, int j)
{
    int t = dest->n - i; /*длинна хвоста*/
    memmove(dest->key + i, src->key + j, t * sizeof(int));
    memmove(dest->child + i, src->child + j, (t + 1) * sizeof(b_tree *));
}

/*разбиение c-го ребенка p*/
static int split(int c, b_tree *p, int t)
{
    b_tree *right_c = p->child[c]; /*правая отделившеяся часть*/
    b_tree *left_c = create(t);    /*левая отделившеяся часть*/
    if (left_c == NULL)
    {
        return ENOMEM;
    }

    right_c->n = t - 1; /*половина от макс количества ключей*/
    left_c->n = t - 1;  /*половина от макс количества ключей*/

    move_tail(left_c, 0, right_c, t); /*левую половину из правого в левого */

    p->n += 1; /*+1 так как из ребенка прийдет средний элемент*/

    move_tail(p, c + 1, p, c); /*сдвиг элементов p на 1, чтобы освободить место для среднего элемената из c ребенка*/

    p->key[c] = right_c->key[t - 1]; /*middle*/
    p->child[c + 1] = left_c;        /*с + 1 ребенок равен правой отделившейся части*/
    return EXIT_SUCCESS;
}

static void add_key(b_tree *bt, int k)
{
    int c = get_child_idx(bt, k);
    bt->n += 1;
    move_tail(bt, c + 1, bt, c); /* освобождаем место под ключ*/
    bt->key[c] = k;
}

static int insert_non_full(b_tree *bt, int t, int k)
{
    if (is_leaf(bt))
    {
        add_key(bt, k);
    }
    else
    {
        int child_idx = get_child_idx(bt, k); /*в какого ребенка добавить?*/
        if (is_full(bt->child[child_idx], t))
        {
            int return_value = split(child_idx, bt, t);
            if (return_value)
            {
                return return_value;
            }
        }
        return insert_non_full(bt->child[get_child_idx(bt, k)], t, k);
    }
    return EXIT_SUCCESS;
}

static int insert(b_tree **bt, int t, int k)
{
    if (*bt == NULL)
    {
        *bt = create(t);
        if (*bt == NULL)
        {
            return ENOMEM;
        }
        (*bt)->key[0] = k;
        (*bt)->n = 1;
    }
    else
    {
        if (is_full(*bt, t)) /*только для корня*/
        {
            b_tree *new_root = create(t);
            if (*bt == NULL)
            {
                return ENOMEM;
            }
            new_root->child[0] = *bt;
            int return_value = split(0, new_root, t);
            if (return_value)
            {
                return return_value;
            }
            *bt = new_root;
        }
        return insert_non_full(*bt, t, k);
    }
    return EXIT_SUCCESS;
}

int input_tree(b_tree **bt, int t, int tree_size)
{
    *bt = NULL;
    for (int i = 0; i < tree_size; ++i)
    {
        int k = 0;
        if (scanf("%d", &k) < 1)
        {
            DEBUG_PRINT("Input error");
            return EIO;
        }

        int return_value = insert(bt, t, k);
        if (return_value)
        {
            return return_value;
        }
    }
    return EXIT_SUCCESS;
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
