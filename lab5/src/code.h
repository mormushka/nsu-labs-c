#ifndef CODE_H
#define CODE_H

#include <stdlib.h>
#include "constants.h"
#include "bit_stream.h"
#include "tree.h"

typedef struct code
{
    unsigned long long code;
    unsigned int length;
} code;

code *make_code_table(tree_node *root);
void pack(unsigned char c, code *codes, bit_stream *stream);

#endif
