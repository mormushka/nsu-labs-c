#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERN_LEN 16
#define BUFFER_SIZE MAX_PATTERN_LEN

typedef struct {
    char str[MAX_PATTERN_LEN + 2];
    int len;
    unsigned hash;
    char bad;
} search_pattern;

typedef struct {
    char data[BUFFER_SIZE];
    int head;
} rolling_buffer;

typedef struct {
    rolling_buffer r_buffer;
    int len;
    unsigned hash;
    char bad;
    size_t index;
    FILE* in;
    unsigned mult_last_char;
} search_field;

int exp1(int num, int d) { return (d == 0) ? (1) : (num * exp1(num, d - 1)); }

unsigned hash_str(const char* str, int len) { //H(C0, ..., CN-1) = СУММА (Ci % 3) * 3^i
    unsigned hash = 0;
    for (int i = len - 1; i >= 0; --i)
        hash = hash * 3 + ((unsigned char)str[i] % 3);
    return hash;
}

search_pattern create_s_pattern(FILE* in) {
    search_pattern tmp = {.str = {0}};
    if (fgets(tmp.str, MAX_PATTERN_LEN + 2, in) == 0) tmp.bad = 1; // +2 чтобы даже при макс длинне паттерна прочитать \n
    tmp.len = strlen(tmp.str) - 1;
    tmp.hash = hash_str(tmp.str, tmp.len);
    return tmp;
}

search_field create_s_window(search_pattern* p, FILE* in) {
    search_field tmp = {.r_buffer.data = {0}};
    if (!fread(tmp.r_buffer.data, 1, p->len, in)) tmp.bad = 1;
    tmp.len = tmp.r_buffer.head = tmp.index = p->len;
    tmp.hash = hash_str(tmp.r_buffer.data, p->len);
    tmp.mult_last_char = exp1(3, p->len - 1);
    tmp.in = in;
    return tmp;
}

void shift_hash( search_field* w ) {
    w->hash = (w->hash - ((unsigned char)w->r_buffer.data[(w->index - w->len) % BUFFER_SIZE] % 3)) / 3\
    + ((unsigned char)w->r_buffer.data[(w->index) % BUFFER_SIZE] % 3) * w->mult_last_char;
}

char print_match(const search_pattern* p, const search_field* w) {
    for (int i = 0; i < p->len; ++i) {
        printf("%zu ", w->index - p->len + i + 1);
        if (p->str[i] != w->r_buffer.data[(w->index - w->len + i) % BUFFER_SIZE])  
            return 0;
    }
    return 1;
}

void move_forward(search_field* w) {
    w->r_buffer.data[w->r_buffer.head] = fgetc(w->in);
    w->r_buffer.head = (w->r_buffer.head + 1) % BUFFER_SIZE;
    
    shift_hash(w);
    ++w->index;
}


void find_substring(FILE* in) {
    search_pattern p = create_s_pattern(in);
    search_field w = create_s_window(&p, in);

    printf("%u ", p.hash);
    if (p.bad || w.bad) return;

    while (!feof(in)) {
        if (p.hash == w.hash)
            print_match(&p, &w);
        move_forward(&w);
    }
}

int main()
{
    FILE* in = fopen("in.txt", "r");
    if (in == NULL) exit(0);

    find_substring(in);
    
    exit(0);
}