#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct trie
{
    char *key;
    int len;
    bool is_key;
    struct trie *link;
    struct trie *next;
} trie;

typedef struct find_info
{
    struct trie *value;
    int match_len;
} find_info;

trie *create(char *sample_prefix, int len, bool is_key)
{
    trie *tmp = calloc(1, sizeof(trie));
    if (tmp == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return NULL;
    }
    tmp->is_key = is_key;
    tmp->len = len;
    tmp->key = calloc(len, sizeof(char));
    if (tmp->key == NULL)
    {
        free(tmp);
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
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
void split(trie *t, int split_point)
{
    trie *tail = create(t->key + split_point, t->len - split_point, t->is_key);
    tail->link = t->link;
    t->link = tail;

    t->key[split_point] = '\0';
    t->len = split_point;
}

void insert(trie **t, char *sample_prefix, int str_len)
{
    if (*sample_prefix == '\0')
    {
        return;
    }

    if (!(*t))
    {
        (*t) = create(sample_prefix, str_len, true);
        return;
    }

    int len_common_pr = prefix(sample_prefix, str_len, (*t)->key, (*t)->len);
    if (len_common_pr == 0)
    {
        insert(&(*t)->next, sample_prefix, str_len);
    }
    else
    {
        if (len_common_pr != (*t)->len)
        {
            split(*t, len_common_pr);
            (*t)->is_key = (len_common_pr == str_len);
        }
        insert(&(*t)->link, sample_prefix + len_common_pr, str_len - len_common_pr);
    }
}

void input_tree(trie **root, int count)
{
    *root = NULL;
    char *sample_prefix = calloc(10001, sizeof(char));
    if (sample_prefix == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        // return NULL;
    }

    for (int i = 0; i < count; ++i)
    {
        if (!scanf("%10000s", sample_prefix))
        {
            fprintf(stderr, "Input error %d\n", __LINE__);
        }
        insert(root, sample_prefix, strlen(sample_prefix));
    }
    free(sample_prefix);
}

void print_childrens_h(char *pr, int len_pr, trie *t)
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

find_info find(trie *t, char *sample, int len)
{
    while(t)
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

void print_childrens(trie *t, char *sample_prefix)
{
    char *pr = calloc(10001, sizeof(char));
    if (pr == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
    }
    strcpy(pr, sample_prefix);

    int len_pr = strlen(sample_prefix);
    find_info f_inf = find(t, sample_prefix, len_pr);
    if (!f_inf.value)
    {
        printf("None");
        free(pr);
        return;
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
}

size_t check_num_nods_h(trie *t)
{
    if (!t)
    {
        return 0;
    }
    return 1 + check_num_nods_h(t->next) + check_num_nods_h(t->link);
}

size_t check_num_nods(trie *t)
{
    return t ? (t->next ? 1 + check_num_nods_h(t) : check_num_nods_h(t)) : 0;
}

void destroy_trie(trie *t)
{
    if (!t)
    {
        return;
    }

    destroy_trie(t->next);
    destroy_trie(t->link);

    free(t->key);
    free(t);
}

int main()
{
    // freopen("../in.txt", "r", stdin);
    int count = 0;
    if (scanf("%d", &count) < 1)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }
    trie *tree;
    input_tree(&tree, count);

    char *sample_prefix = calloc(10001, sizeof(char));
    if (!scanf("%10000s", sample_prefix))
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
    }

    print_childrens(tree, sample_prefix);
    printf("\n");

    free(sample_prefix);

    printf("%zu\n", check_num_nods(tree));

    destroy_trie(tree);

    return EXIT_SUCCESS;
}
