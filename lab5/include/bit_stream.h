#ifndef BIT_STREAM_H
#define BIT_STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "debug_macros.h"
#include "constants.h"

typedef struct tbit_stream
{
    FILE *file;
    unsigned char data;
    int pos;
} tbit_stream;

tbit_stream *create_bit_stream(FILE *file);
int read_bit(tbit_stream *bit_stream, int *bit);
int read_byte(tbit_stream *bit_stream, unsigned char *byte);
int write_bit(int bit, tbit_stream *bit_stream);
int write_byte(unsigned char byte, tbit_stream *bit_stream);
int flush(tbit_stream *bit_stream);

#endif
