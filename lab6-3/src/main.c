#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "debug_macros.h"

typedef struct pr_tree
{
    char *key;
    int len;
    bool is_key;
    struct pr_tree *link;
    struct pr_tree *next;
} pr_tree;

typedef struct find_info
{
    struct pr_tree *value;
    int match_len;
} find_info;

pr_tree *create(char *sample_prefix, int len, bool is_key)
{
    pr_tree *tmp = calloc(1, sizeof(pr_tree));
    if (tmp == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    tmp->is_key = is_key;
    tmp->len = len;
    tmp->key = calloc(len, sizeof(char));
    if (tmp->key == NULL)
    {
        free(tmp);
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    strncpy(tmp->key, sample_prefix, len);

    return tmp;
}

/* Длина максимального общего префикса */
int prefix(char *a, int a_len, char *b, int b_len)
{
    for (int i = 0; i < a_len; i++)
    {
        if ((i == b_len) || (a[i] != b[i]))
        {
            return i;
        }
    }
    return a_len;
}

/* Отделение хвоста в link с унаследованием is_key и link */
int split(pr_tree *t, int split_point)
{
    pr_tree *tail = create(t->key + split_point, t->len - split_point, t->is_key);
    if (tail == NULL)
    {
        return ENOMEM;
    }
    tail->link = t->link;
    t->link = tail;

    t->key[split_point] = '\0';
    t->len = split_point;
    return EXIT_SUCCESS;
}

int insert(pr_tree **t, char *sample_prefix, int str_len)
{
    if (*sample_prefix == '\0')
    {
        return EXIT_SUCCESS;
    }

    if ((*t) == NULL)
    {
        (*t) = create(sample_prefix, str_len, true);
        if ((*t) == NULL)
        {
            return ENOMEM;
        }
        return EXIT_SUCCESS;
    }

    int len_common_pr = prefix(sample_prefix, str_len, (*t)->key, (*t)->len);
    if (len_common_pr == 0)
    {
        return insert(&(*t)->next, sample_prefix, str_len);
    }
    else
    {
        if (len_common_pr != (*t)->len)
        {
            int return_value = split(*t, len_common_pr);
            if (return_value)
            {
                return return_value;
            }
            (*t)->is_key = (len_common_pr == str_len);
        }
        return insert(&(*t)->link, sample_prefix + len_common_pr, str_len - len_common_pr);
    }
}

int input_tree(pr_tree **root, int count)
{
    *root = NULL;
    char *sample_prefix = calloc(10001, sizeof(char));
    if (sample_prefix == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }

    for (int i = 0; i < count; ++i)
    {
        if (!scanf("%10000s", sample_prefix))
        {
            free(sample_prefix);
            DEBUG_PRINT("Input error");
            return EIO;
        }
        int return_value = insert(root, sample_prefix, strlen(sample_prefix));
        if (return_value)
        {
            free(sample_prefix);
            return return_value;
        }
    }
    free(sample_prefix);
    return EXIT_SUCCESS;
}

void print_childrens_h(char *pr, int len_pr, pr_tree *t)
{
    if (!t)
    {
        return;
    }

    print_childrens_h(pr, len_pr, t->next);

    strncpy(pr + len_pr, t->key, t->len);
    len_pr += t->len;
    pr[len_pr] = '\0';

    if (t->is_key)
    {
        printf("%s ", pr);
    }

    print_childrens_h(pr, len_pr, t->link);
}

find_info find(pr_tree *t, char *sample, int len)
{
    while (t)
    {
        int len_common_pr = prefix(sample, len, t->key, t->len);
        if (len_common_pr == len)
        {
            break;
        }
        if (len_common_pr == 0)
        {
            t = t->next;
            continue;
        }
        if (len_common_pr == t->len)
        {
            t = t->link;
            sample += len_common_pr;
            len -= len_common_pr;
            continue;
        }
        t = NULL;
    }

    find_info return_info = {
        .value = t,
        .match_len = len};

    return return_info;
}

int print_childrens(pr_tree *t, char *sample_prefix)
{
    char *pr = calloc(10001, sizeof(char));
    if (pr == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }
    strcpy(pr, sample_prefix);

    int len_pr = strlen(sample_prefix);
    find_info f_inf = find(t, sample_prefix, len_pr);
    if (!f_inf.value)
    {
        printf("None");
        free(pr);
        return EXIT_SUCCESS;
    }

    if (f_inf.match_len < f_inf.value->len)
    {
        strncpy(pr + len_pr, f_inf.value->key + f_inf.match_len, f_inf.value->len - f_inf.match_len);
        len_pr += f_inf.value->len - f_inf.match_len;
    }

    if (f_inf.value->is_key)
    {
        printf("%s ", pr);
    }

    print_childrens_h(pr, len_pr, f_inf.value->link);
    free(pr);
    return EXIT_SUCCESS;
}

size_t check_num_nods_h(pr_tree *t)
{
    if (!t)
    {
        return 0;
    }
    return 1 + check_num_nods_h(t->next) + check_num_nods_h(t->link);
}

size_t check_num_nods(pr_tree *t)
{
    return t ? (t->next ? 1 + check_num_nods_h(t) : check_num_nods_h(t)) : 0;
}

void destroy_pr_tree(pr_tree *t)
{
    if (!t)
    {
        return;
    }

    destroy_pr_tree(t->next);
    destroy_pr_tree(t->link);

    free(t->key);
    free(t);
}

int main()
{
    // freopen("../in.txt", "r", stdin);
    int count = 0;
    if (scanf("%d", &count) < 1)
    {
        DEBUG_PRINT("Input error");
        return EXIT_FAILURE;
    }

    pr_tree *tree;
    int return_value = input_tree(&tree, count);
    if (return_value)
    {
        destroy_pr_tree(tree);
        return return_value;
    }

    char *sample_prefix = calloc(10001, sizeof(char));
    if (sample_prefix == NULL)
    {
        destroy_pr_tree(tree);
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }

    if (!scanf("%10000s", sample_prefix))
    {
        destroy_pr_tree(tree);
        free(sample_prefix);
        DEBUG_PRINT("Input error");
        return EIO;
    }

    return_value = print_childrens(tree, sample_prefix);
    free(sample_prefix);
    if (return_value)
    {
        destroy_pr_tree(tree);
        return return_value;
    }
    printf("\n");

    printf("%zu\n", check_num_nods(tree));

    destroy_pr_tree(tree);

    return EXIT_SUCCESS;
}
