#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define J_MINUS(J, J_NEGATIVE) {\
    if (J != 0) {\
        J--;\
    } else {\
        J_NEGATIVE = 1;\
        break;\
    }\
}

void swap(int* a, int* b) { 
    int tmp = *a; 
    *a = *b; 
    *b = tmp; 
}

void quick_sort_m(int* a, size_t left, size_t right) {
    if (right > left) {
        char j_negative = 0;
        int p = a[rand() % (right - left + 1) + left];
        size_t i = left, j = right;
        while ((i <= j) && !j_negative) {
            while (a[i] < p) i++;
            while (a[j] > p) J_MINUS(j, j_negative);
            if ((i <= j) && !j_negative) {
                swap(&a[i], &a[j]);
                i++;
                J_MINUS(j, j_negative); 
            }
        }
        quick_sort_m(a, left, j);
        quick_sort_m(a, i, right);
    }
}

void quick_sort(int* a, size_t len) {
    srand(time(NULL));
    if (len == 0) return;
    quick_sort_m(a, 0, len - 1);
}

int main()
{
    FILE* in = fopen("in.txt", "r");
    FILE* out = fopen("out.txt", "w");

    size_t len;
    if (!fscanf(in, "%zu", &len)) {
        fclose(in);
        fclose(out);
        return 0;
    }

    int* a = (int *)malloc(len * sizeof(int));
    for (size_t i = 0; i < len; ++i) {
        if(!fscanf(in, "%d", &a[i])) {    
            free(a);
            fclose(in);
            fclose(out);
            return 0;
        }
    }
    
    quick_sort(a, len);

    for(size_t i = 0; i < len; ++i) 
        fprintf(out, "%d ", a[i]);
    
    free(a);
    fclose(in);
    fclose(out);
    return 0;
}