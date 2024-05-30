#include "progress_bar.h"

void print_progress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    fprintf(stderr, "\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stderr);
}

size_t file_size(FILE *f)
{
    size_t x = ftell(f);
    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);
    fseek(f, x, SEEK_SET);
    return file_size;
}
