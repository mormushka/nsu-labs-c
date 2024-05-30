#include "bit_stream.h"
#include "debug_macros.h"
#include "constants.h"
#include <stdlib.h>
#include <errno.h>

tbit_stream *create_bit_stream(FILE *file)
{
    tbit_stream *bit_stream = malloc(sizeof(struct tbit_stream));
    if (bit_stream == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    bit_stream->file = file;
    bit_stream->data = 0;
    bit_stream->pos = 0;
    return bit_stream;
}

int read_bit(tbit_stream *bit_stream, int *bit)
{
    if (bit_stream->pos == 0)
    {
        if (fread(&bit_stream->data, sizeof(char), 1, bit_stream->file) != 1)
        {
            DEBUG_PRINT("Input error");
            return EIO;
        }
        bit_stream->pos = BUFFER_SIZE;
    }

    bit_stream->pos -= 1;
    *bit = (bit_stream->data >> bit_stream->pos) & 1;
    return EXIT_SUCCESS;
}

int write_bit(const int bit, tbit_stream *bit_stream)
{
    if (bit_stream->pos == BUFFER_SIZE)
    {
        if (fwrite(&bit_stream->data, sizeof(char), 1, bit_stream->file) != 1)
        {
            DEBUG_PRINT("Output error");
            return EIO;
        }
        bit_stream->data = 0;
        bit_stream->pos = 0;
    }

    bit_stream->data = (bit_stream->data << 1) | bit;
    bit_stream->pos += 1;
    return EXIT_SUCCESS;
}

int read_byte(tbit_stream *bit_stream, unsigned char *byte)
{
    *byte = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        *byte = *byte << 1;
        int bit;
        if (read_bit(bit_stream, &bit))
        {
            DEBUG_PRINT("");
            return EIO;
        }
        *byte = *byte | bit;
    }

    return EXIT_SUCCESS;
}

int write_byte(const unsigned char byte, tbit_stream *bit_stream)
{
    for (int i = BUFFER_SIZE - 1; i >= 0; i--)
    {
        int bit = (byte >> i) & 1;
        if (write_bit(bit, bit_stream))
        {
            DEBUG_PRINT("");
            return EIO;
        }
    }
    return EXIT_SUCCESS;
}

int flush(tbit_stream *bit_stream)
{
    bit_stream->data = bit_stream->data << (BUFFER_SIZE - bit_stream->pos);
    if (fwrite(&bit_stream->data, sizeof(char), 1, bit_stream->file) != 1)
    {
        DEBUG_PRINT("Output error");
        return EIO;
    }
    return EXIT_SUCCESS;
}
