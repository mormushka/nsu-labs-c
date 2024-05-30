#ifndef CODE_H
#define CODE_H

#include "bit_stream.h"
#include "tree.h"
#include "constants.h"

typedef struct code
{
    char code[ALPHABET_SIZE];
    int length;
} code;

code *make_code_table(ttree *root);
int pack(const int c, code *codes, tbit_stream *bit_stream);
#endif
