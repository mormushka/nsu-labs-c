#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

#define ON_PROGRESS_BAR
#define ON_DIF_TIME11
#define NDEBUG11

void print_usage();
int encode(FILE *in, FILE *out, char terminal_mode);
int decode(FILE *in, FILE *out, char terminal_mode);

#endif
