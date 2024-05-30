#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

void print_usage();
int encode(FILE *in, FILE *out, char terminal_mode);
int decode(FILE *in, FILE *out, char terminal_mode);

#endif
