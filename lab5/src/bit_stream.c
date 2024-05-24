#include "bit_stream.h"

bit_stream *create_bit_stream(FILE *file)
{
    bit_stream *stream = malloc(sizeof(struct bit_stream));
    if (stream == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    stream->file = file;
    stream->data = 0;
    stream->pos = 0;
    return stream;
}

int read_bit(bit_stream *stream, int *bit)
{
    if (stream->pos == 0)
    {
        if (fread(&stream->data, sizeof(char), 1, stream->file) != 1)
        {
            DEBUG_PRINT("Input error");
            return EIO;
        }
        stream->pos = BUFFER_SIZE;
    }

    stream->pos -= 1;
    *bit = (stream->data >> stream->pos) & 1;
    return EXIT_SUCCESS;
}

int write_bit(const int bit, bit_stream *stream)
{
    if (stream->pos == BUFFER_SIZE)
    {
        if (fwrite(&stream->data, sizeof(char), 1, stream->file) != 1)
        {
            DEBUG_PRINT("Output error");
            return EIO;
        }
        stream->data = 0;
        stream->pos = 0;
    }

    stream->data = (stream->data << 1) | bit;
    stream->pos += 1;
    return EXIT_SUCCESS;
}

int read_byte(bit_stream *stream, unsigned char *byte)
{
    *byte = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        *byte = *byte << 1;
        int bit;
        if (read_bit(stream, &bit))
        {
            return EIO;
        }
        *byte = *byte | bit;
    }

    return EXIT_SUCCESS;
}

int write_byte(const unsigned char byte, bit_stream *stream)
{
    for (int i = BUFFER_SIZE - 1; i >= 0; i--)
    {
        int bit = (byte >> i) & 1;
        if (write_bit(bit, stream))
        {
            return EIO;
        }
    }
    return EXIT_SUCCESS;
}

int flush(bit_stream *stream)
{
    stream->data = stream->data << (BUFFER_SIZE - stream->pos);
    if (fwrite(&stream->data, sizeof(char), 1, stream->file) != 1)
    {
        DEBUG_PRINT("Output error");
        return EIO;
    }
    return EXIT_SUCCESS;
}
