#include <stdio.h>
#include <string.h>

enum {max_pattern_len = 16, ASCII_len = 256};

typedef struct SEARCH_PATTERN {
    char str[max_pattern_len + 2];
    short len;
    int stop_shift[ASCII_len];
    char bad;
} search_pattern;

typedef struct SEARCH_FIELD {
    char str[max_pattern_len + 1];
    size_t index;
    FILE* input;
    size_t s_field_size;
    char last_char;
    char bad;
} search_field;

void fill_array_end_char(char* ar, short length) { 
    for (short i = 0; i < length - 1  ; ++i) ar[i] = '\0'; 
}

char print_match(const search_pattern* pattern, const search_field* window) {
    for (int i = pattern->len - 1; i >= 0; --i) {
        printf("%zu ", window->index + i + 1);
        if (pattern->str[i] != window->str[i])  
            return 0;
    }
    return 1;
}

int calc_shift(const search_pattern* pattern, const search_field* window) {
    return pattern->stop_shift[128 + (int)window->last_char];
}

void move_forward(search_field* window, int shift, const search_pattern* pattern) {
    char buffer[16];
    fill_array_end_char(buffer, 16);

    for (int i = 0; i < pattern->len - shift; ++i) // запись в buffer
        buffer[i] = window->str[pattern->len - 1 - i];
    for (int i = pattern->len - shift - 1; i >= 0; --i) // запись в window из buffer
        window->str[pattern->len - shift - 1 - i] = buffer[i];
    for (int i = 0; i < shift; ++i) // запись в window из stdin
        window->str[pattern->len - shift + i] = fgetc(window->input);

    window->last_char = window->str[pattern->len - 1];
    window->index += shift;
}

void find_substring(const search_pattern* pattern, search_field* window) {
    while (!feof(window->input)) {
        print_match(pattern, window);
        move_forward(window, calc_shift(pattern, window), pattern);
    }
}

void fill_stop_shift_table(search_pattern* pattern) {
    for (short i = 0; i < ASCII_len; ++i) pattern->stop_shift[i] = pattern->len;
    int j = 1;
    for (int i = pattern->len - 2; i >= 0; --i, ++j) {
        if (pattern->stop_shift[128 + (int)pattern->str[i]] == pattern->len)
            pattern->stop_shift[128 + (int)pattern->str[i]] = j;
    }
}

size_t file_size(FILE* input) {
    fseek(input, 0, SEEK_END);
    int file_size = ftell(input);
    fseek(input, 0, SEEK_SET);
    return file_size;
}

search_pattern create_s_pattern(FILE* input) {
    search_pattern tmp = {.str = "\0"};
    if (fgets(tmp.str, max_pattern_len + 2, input) == 0)  tmp.bad = 1; // +2 чтобы даже при макс длинне паттерна прочитать \n
    tmp.len = strlen(tmp.str) - 1;
    tmp.str[tmp.len] = '\0';
    fill_stop_shift_table(&tmp);
    return tmp;
}

search_field create_s_window(search_pattern* pattern, size_t f_len, FILE* input) {
    search_field tmp = {.str = "\0"};
    if (fgets(tmp.str, pattern->len + 1, input) == 0) tmp.bad = 1;
    tmp.last_char = tmp.str[pattern->len - 1];
    tmp.s_field_size = f_len - (pattern->len + 1);
    tmp.input = input;
    return tmp;
}


int main()
{
    FILE* input = fopen("in.txt", "r");
    size_t f_len = file_size(input);

    search_pattern pattern = create_s_pattern(input);
    search_field window = create_s_window(&pattern, f_len, input);

    if ((pattern.bad + window.bad) > 0) {
        fclose(input);
        return 0;
    }

    find_substring(&pattern, &window);
    
    fclose(input);
    return 0;
} 