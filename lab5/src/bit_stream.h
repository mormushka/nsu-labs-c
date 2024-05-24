#ifndef BIT_STREAM_H
#define BIT_STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

typedef struct bit_stream
{
    FILE *file;
    unsigned char data;
    int pos;
} bit_stream;

bit_stream *create_bit_stream(FILE *file);
error_code read_bit(bit_stream *stream, int *bit);
error_code read_byte(bit_stream *stream, unsigned char *byte);
void write_bit(int bit, bit_stream *stream);
void write_byte(unsigned char byte, bit_stream *stream);
void flush(bit_stream *stream);

#endif
