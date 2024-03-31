#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 10

void swap(char* a, char* b) { char tmp = *a; *a = *b; *b = tmp; }

int is_good(char* str, int len) {
    char num_nums[10] = {0};
    for (int i = 0; i < len; ++i) {
        if (!( ('0' <= str[i]) && (str[i] <= '9') ))
            return 0;
        if (num_nums[str[i] - '0']++)
            return 0;
    }
    return 1;
}

void reverse (char* str, int start_i, int end_i) {
    for (; start_i < end_i; start_i++, end_i--) 
        swap(&str[start_i], &str[end_i]);
}

int search_i (char* str, int len) {
    for (int i = len - 2; i >= 0; --i)
        if (str[i] < str[i + 1])
            return i;
    return -1;
}

int search_j (char* str, int len, int i) {
    for(int j = len - 1; j >= 0; --j)
        if(str[j] > str[i])
            return j;
    return -1;
}

int perm(char* str, int len) {
    int i, j;
    if ((i = search_i(str, len)) == -1) return 0;
    if ((j = search_j(str, len, i)) == -1) return 0;
    swap(&str[i], &str[j]);
    reverse (str, i + 1, len - 1);
    return 1;
}

int perms(char* str, int len, int num) {
    if (!is_good(str, len))
        return 0;
    for(int i = 0; i < num; ++i) {
        if(!perm(str, len)) 
            break;
        printf("%s\n", str);
    }
    return 1;
}

int main()
{
    FILE* in = fopen("in.txt", "r");
    if (in == NULL) 
        exit(0);

    char str[MAX_INPUT + 2];
    int n;

    if (!fscanf(in, "%11[^\n]", str)) {
        printf("bad input");
        exit(0);
    }
    int len = strlen(str);

    if (!is_good(str, len) || !fscanf(in, "%d", &n) ) { 
        printf("bad input");
        exit(0);
    }

    if (!perms(str, len, n)) {
        printf("bad input");
        exit(0);
    }

    exit(0);
}