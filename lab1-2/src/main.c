#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_P_LEN 16
#define BUFFER_SIZE 32

typedef struct
{
    size_t data[BUFFER_SIZE];
    size_t first;
    size_t free_block;
    int count;
} queue;

void queue_add(queue *q, size_t index, size_t prefix_len)
{
    q->data[q->free_block++] = index;
    q->data[q->free_block++] = prefix_len;
    q->free_block %= BUFFER_SIZE;
    q->count++;
}

void del_first(queue *q)
{
    q->first = (q->first + 2) % BUFFER_SIZE;
    q->count--;
}

void fill_shift_table(int *shift_table, const char *pattern, int len)
{
    int i = 1;
    int j = 0;
    while (len > i)
    {
        if (pattern[j] == pattern[i])
        {
            shift_table[i] = ++j;
            ++i;
        }
        else
        {
            if (j == 0)
            {
                shift_table[i] = 0;
                ++i;
            }
            else
            {
                j = shift_table[j - 1];
            }
        }
    }
}

void kmp(FILE *in)
{
    char pattern[MAX_P_LEN + 2] = {0};
    if (fgets(pattern, MAX_P_LEN + 2, in) == 0)
    {
        return;
    }

    int len = 0;
    len = strlen(pattern) - 1;
    if (len == 0)
    {
        return;
    }

    int shift_table[MAX_P_LEN] = {0};
    fill_shift_table(shift_table, pattern, len);

    for (int i = 0; i < len; ++i)
    {
        printf("%d ", shift_table[i]);
    }

    queue q = {
        .count = 0,
        .first = 0,
        .free_block = 0
    };
    size_t i = 1;
    int j = 0;
    int c = fgetc(in);
    while (c != EOF)
    {
        if (pattern[j] == (char)c)
        {
            ++i;
            ++j;
            c = fgetc(in);
        }
        else
        {
            if (j > 0)
            {
                queue_add(&q, i - j, j);
                j = shift_table[j - 1];
                while (q.count && (q.data[q.first] <= i - len))
                {
                    printf("%zu %zu ", q.data[q.first], q.data[q.first + 1]);
                    del_first(&q);
                }
            }
            else
            {
                ++i;
                c = fgetc(in);
            }
        }
    }

    if (j == len)
    {
        queue_add(&q, i - j, j);
    }
    while (q.count && (q.data[q.first] <= i - len))
    {
        printf("%zu %zu ", q.data[q.first], q.data[q.first + 1]);
        del_first(&q);
    }
}

int main()
{
    FILE *in = fopen("in.txt", "r");
    if (in == NULL)
    {
        return EXIT_FAILURE;
    }
    kmp(in);

    fclose(in);
    return EXIT_SUCCESS;
}