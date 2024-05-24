#include "huffman.h"

void print_usage()
{
    printf("Usage: huffman.exe <command> <file.in> <file.out>\n");
    printf("<command> - symbol of command: c - compress, d - decompress\n");
    printf("<file.in> - input file\n");
    printf("<file.out> - output file\n");
}

int *create_frequency_table(FILE *file)
{
    int *frequencies = calloc(ALPHABET_SIZE, sizeof(int));
    if (frequencies == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    unsigned char curr_symbol = fgetc(file);
    while (!feof(file))
    {
        frequencies[curr_symbol] += 1;
        curr_symbol = fgetc(file);
    }

    return frequencies;
}

int encode(FILE *in, FILE *out, char terminal_mode)
{
    if (fgetc(in) == EOF)
    {
        return EXIT_SUCCESS;
    }
    else
    {
        fseek(in, 1 - terminal_mode, SEEK_SET);
    }

    bit_stream *stream = create_bit_stream(out);
    int *frequencies = create_frequency_table(in);
    fseek(in, 1 - terminal_mode, SEEK_SET);

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
        free(stream);
        free(codes);
        free(frequencies);
        return ENOMEM;
    }

    int length = root->freq;
    if (fwrite(&length, sizeof(int), 1, out) != 1)
    {
        destroy_tree(root);
        free(stream);
        free(codes);
        free(frequencies);
        DEBUG_PRINT("Output error");
        return EIO;
    }

    if (pack_tree(root, stream))
    {
        destroy_tree(root);
        free(stream);
        free(codes);
        free(frequencies);
        return EIO;
    }

    while (!feof(in))
    {
        unsigned char c = fgetc(in);
        if (feof(in))
        {
            break;
        }

        if (pack(c, codes, stream))
        {
            free(stream);
            destroy_tree(root);
            free(codes);
            free(frequencies);
            return EIO;
        }
    }
    destroy_tree(root);
    free(codes);
    free(frequencies);
    if (flush(stream))
    {
        free(stream);
        return EIO;
    }
    free(stream);
    return EXIT_SUCCESS;
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
        if (unpack(root, stream, &c))
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
