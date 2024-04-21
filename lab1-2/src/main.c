#include <stdio.h>
#include <stdlib.h>

#define MAX_SAMPLE_LEN 16
#define MAX_STR_LEN 256

typedef struct t_string
{
    int index;
    int len;
    char *line;
} t_string;

t_string create_string(int len);
int input_sample(t_string *str);
void input_str(t_string *str);
void prefix_function(int *shift_table, t_string *sample);
void print_shift_table(int sample_len, int *shift_table);
void print_protokol(int position, int len);
int shift_str(int shift, int sample_len, t_string *string);
int kmp(t_string *sample);

int main()
{
    //FILE *in = freopen("../in.txt", "r", stdin);
    t_string sample = create_string(MAX_SAMPLE_LEN);
    if (sample.line == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
    }

    if (input_sample(&sample) == EXIT_FAILURE)
    {
        free(sample.line);
        return EXIT_FAILURE;
    }

    if (kmp(&sample) == EXIT_FAILURE)
    {
        free(sample.line);
        return EXIT_FAILURE;
    }

    free(sample.line);
    return EXIT_SUCCESS;
}

t_string create_string(int len)
{
    t_string tmp = {
        .index = 0,
        .len = 0,
        .line = (char *)malloc(len * sizeof(char))};

    return tmp;
}

int input_sample(t_string *str)
{
    for (int i = 0; i < MAX_SAMPLE_LEN + 1; ++i)
    {
        int c = 0;
        if (fread(&c, sizeof(char), 1, stdin) != 1)
        {
            fprintf(stderr, "Input error %d\n", __LINE__);
            return EXIT_FAILURE;
        }

        if (c == '\n')
        {
            str->len = i;
            return EXIT_SUCCESS;
        }

        str->line[i] = c;
    }

    fprintf(stderr, "Sample lenght > MAX_SAMPLE_LEN %d\n", __LINE__);
    return EXIT_FAILURE;
}

void input_str(t_string *str)
{
    str->len = (int)fread(str->line, sizeof(char), MAX_STR_LEN, stdin);
    str->index = 0;
}

void prefix_function(int *shift_table, t_string *sample)
{
    for (int i = 1; i < sample->len; ++i)
    {
        int j = shift_table[i - 1];
        while ((j > 0) && (sample->line[i] != sample->line[j]))
        {
            j = shift_table[j - 1];
        }

        if (sample->line[i] == sample->line[j])
        {
            ++j;
        }

        shift_table[i] = j;
    }
}

void print_shift_table(int sample_len, int *shift_table)
{
    for (int i = 0; i < sample_len; ++i)
    {
        printf("%d ", shift_table[i]);
    }
}

void print_protokol(int position, int len)
{
    printf("%d %d ", position, len);
}

int shift_str(int shift, int sample_len, t_string *string)
{
    if (string->index + sample_len + shift> string->len)
    {
        int shifted_sum_count = string->len - string->index - shift;
        for (int i = 0; i < shifted_sum_count; ++i)
        {
            string->line[i] = string->line[string->index + shift + i];
        }

        string->len = shifted_sum_count + (int)fread(string->line + shifted_sum_count, sizeof(char), MAX_STR_LEN - shifted_sum_count, stdin);
        string->index = 0;
        return (string->len > sample_len);
    }

    string->index += shift;
    return 1;
}

int kmp(t_string *sample)
{
    if (sample->len == 0)
    {
        fprintf(stderr, "Sample lenght = 0 %d\n", __LINE__);
        return EXIT_SUCCESS;
    }

    int shift_table[MAX_SAMPLE_LEN] = {0};
    prefix_function(shift_table, sample);
    print_shift_table(sample->len, shift_table);

    t_string str = create_string(MAX_STR_LEN);
    if (str.line == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    input_str(&str);

    if (sample->len > str.len)
    {
        free(str.line);
        return EXIT_SUCCESS;
    }

    int position = 1;

    for (;;)
    {
        int prefix_len = 0;
        while ((prefix_len < sample->len) && (sample->line[prefix_len] == str.line[str.index + prefix_len]))
        {
            ++prefix_len;
        }

        int shift = 1;
        if (prefix_len != 0)
        {
            shift = prefix_len - shift_table[prefix_len - 1];
            print_protokol(position, prefix_len);
        }

        position += shift;
        if (!shift_str(shift, sample->len, &str))
        {
            free(str.line);
            return EXIT_SUCCESS;
        }
    }
}