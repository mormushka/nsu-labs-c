#ifndef BIT_STREAM_H
#define BIT_STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "constants.h"
#include "debug_macros.h"

typedef struct bit_stream
{
    FILE *file;
    unsigned char data;
    int pos;
} bit_stream;

bit_stream *create_bit_stream(FILE *file);
int read_bit(bit_stream *stream, int *bit);
int read_byte(bit_stream *stream, unsigned char *byte);
int write_bit(int bit, bit_stream *stream);
int write_byte(unsigned char byte, bit_stream *stream);
int flush(bit_stream *stream);

#endif
