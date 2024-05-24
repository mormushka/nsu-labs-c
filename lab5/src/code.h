#ifndef CODE_H
#define CODE_H

#include <stdlib.h>
#include "constants.h"
#include "bit_stream.h"
#include "tree.h"
#include "debug_macros.h"

typedef struct code
{
    unsigned long long code;
    unsigned int length;
} code;

code *make_code_table(ttree *root);
int pack(unsigned char c, code *codes, tbit_stream *bit_stream);

#endif
