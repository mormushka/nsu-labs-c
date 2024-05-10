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

trie *create(char *str, int len, bool is_key)
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
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return NULL;
    }
    strncpy(tmp->key, str, len);

    return tmp;
}

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

void split(trie *t, int split_point)
{
    trie *separated_part = create(t->key + split_point, t->len - split_point, t->is_key);
    separated_part->link = t->link;
    t->link = separated_part;

    char *a = calloc(split_point, sizeof(char));
    if (a == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
    }
    strncpy(a, t->key, split_point);
    free(t->key);

    t->key = a;
    t->len = split_point;
}

trie *insert(trie *t, char *str, int str_len)
{
    if (*str == '\0')
    {
        return t;
    }

    if (!t)
    {
        return create(str, str_len, true);
    }

    int len_common_pr = prefix(str, str_len, t->key, t->len);
    if (len_common_pr == 0)
    {
        t->next = insert(t->next, str, str_len);
    }
    else if (len_common_pr <= str_len)
    {
        if (len_common_pr < t->len)
        {
            split(t, len_common_pr);
            if (len_common_pr != str_len)
            {
                t->is_key = false;
            }
        }
        t->link = insert(t->link, str + len_common_pr, str_len - len_common_pr);
    }
    return t;
}

trie *input_tree(int count)
{
    trie *tree = NULL;
    char *str = calloc(10001, sizeof(char));
    if (str == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return NULL;
    }

    for (int i = 0; i < count; ++i)
    {
        if (!scanf("%10000s", str))
        {
            fprintf(stderr, "Input error %d\n", __LINE__);
        }
        tree = insert(tree, str, strlen(str));
    }
    free(str);

    return tree;
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

void print_childrens(trie *t, char *str)
{
    char *pr = calloc(10001, sizeof(char));
    if (pr == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
    }
    strcpy(pr, str);

    int len_pr = strlen(str);
    int n = len_pr;
    for (;;)
    {
        if (!t)
        {
            break;
        }

        int len_common_pr = prefix(str, n, t->key, t->len);
        if (len_common_pr == 0)
        {
            t = t->next;
            continue;
        }

        if (len_common_pr == n)
        {
            break;
        }

        if (len_common_pr == t->len)
        {
            t = t->link;
            str += len_common_pr;
            n -= len_common_pr;
            continue;
        }

        t = NULL;
        break;
    }

    if (!t)
    {
        printf("None");
        free(pr);
        return;
    }

    if (n < t->len)
    {
        strncpy(pr + len_pr, t->key + n, t->len - n);
        len_pr += t->len - n;
    }

    if (t->is_key)
    {
        printf("%s ", pr);
    }

    print_childrens_h(pr, len_pr, t->link);
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
    int count = 0;
    if (scanf("%d", &count) <= 0)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }
    trie *tree = input_tree(count);

    char *f = calloc(10001, sizeof(char));
    if (!scanf("%10000s", f))
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
    }

    print_childrens(tree, f);
    printf("\n");

    free(f);

    printf("%zu\n", check_num_nods(tree));

    destroy_trie(tree);

    return EXIT_SUCCESS;
}
