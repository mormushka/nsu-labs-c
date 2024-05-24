#include "huffman.h"

void print_usage()
{
    printf("Usage: huffman.exe <command> <file.in> <file.out>\n");
    printf("<command> - symbol of command: c - compress, d - decompress\n");
    printf("<file.in> - input file\n");
    printf("<file.out> - output file\n");
}

int *create_frequency_table(FILE *in)
{
    int *frequencies = calloc(ALPHABET_SIZE, sizeof(int));
    if (!frequencies)
    {
        return NULL;
    }
    unsigned char curr_symbol = fgetc(in);
    while (!feof(in))
    {
        frequencies[curr_symbol] += 1;
        curr_symbol = fgetc(in);
    }

    return frequencies;
}

void encode(FILE *raw, FILE *zipped, char terminal_mode)
{
    if (fgetc(raw) == EOF)
    {
        return;
    }
    else
    {
        fseek(raw, 1 - terminal_mode, SEEK_SET);
    }

    bit_stream *stream = create_bit_stream(zipped);
    int *frequencies = create_frequency_table(raw);
    fseek(raw, 1 - terminal_mode, SEEK_SET);

#ifndef NDEBUG
    fprintf(stderr, "# FREQUENCY TABLE:\n");
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (frequencies[i])
        {
            fprintf(stderr, "%.2x - %d\n", i, frequencies[i]);
        }
    }
    fprintf(stderr, "# END FREQUENCY TABLE\n\n");
#endif

    tree_node *root = create_tree(frequencies);
    code *codes = make_code_table(root);

#ifndef NDEBUG
    fprintf(stderr, "# CODES TABLE:\n");
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        unsigned long long k = codes[i].code;
        if (codes[i].length)
        {
            fprintf(stderr, "%.2x - ", i);
            for (int j = 0; j < codes[i].length; j++)
            {
                fprintf(stderr, "%llu", k & 1);
                k = (k >> 1);
            }
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "# END CODES TABLE\n\n");
#endif

    if (!stream || !frequencies || !root || !codes)
    {
        destroy_tree(root);
        free(codes);
        free(frequencies);
        free(stream);
        return;
    }
    int length = root->freq;
    fwrite(&length, sizeof(int), 1, zipped);

    pack_tree(root, stream);
    while (!feof(raw))
    {
        unsigned char c = fgetc(raw);
        if (feof(raw))
        {
            break;
        }

        pack(c, codes, stream);
    }
    flush(stream);
    destroy_tree(root);
    free(codes);
    free(frequencies);
    free(stream);
}

void decode(FILE *zipped, FILE *unzipped, char terminal_mode)
{
    if (fgetc(zipped) == EOF)
    {
        return;
    }
    else
    {
        fseek(zipped, 1 - terminal_mode, SEEK_SET);
    }

    bit_stream *stream = create_bit_stream(zipped);
    if (!stream)
    {
        return;
    }

    int length;
    if (fread(&length, sizeof(int), 1, zipped) != 1)
    {
        return;
    }

    tree_node *root = unpack_tree(stream);
    if (!root)
    {
        free(stream);
        return;
    }
    for (int i = 0; i < length; i++)
    {
        unsigned char c;
        if (unpack(root, stream, &c) == read_error)
        {
            destroy_tree(root);
            free(stream);
            return;
        }
        fwrite(&c, sizeof(char), 1, unzipped);
    }

    destroy_tree(root);
    free(stream);
}
