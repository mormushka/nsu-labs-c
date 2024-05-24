#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "bit_stream.h"
#include "code.h"
#include "constants.h"
#include "tree.h"
#include "debug_macros.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void print_usage();
int encode(FILE *raw, FILE *zipped, char terminal_mode);
int decode(FILE *zipped, FILE *unzipped, char terminal_mode);

#endif
