#include "huffman.h"

void print_usage()
{
    printf("Usage: huffman.exe <command> <file.in> <file.out>\n");
    printf("<command> - symbol of command: c - compress, d - decompress\n");
    printf("<file.in> - input file\n");
    printf("<file.out> - output file\n");
}

unsigned *calc_hist(FILE *file)
{
    unsigned *hist = calloc(ALPHABET_SIZE, sizeof(int));
    if (hist == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    unsigned char curr_symbol = fgetc(file);
    while (!feof(file))
    {
        hist[curr_symbol] += 1;
        curr_symbol = fgetc(file);
    }

    return hist;
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

    tbit_stream *bit_stream = create_bit_stream(out);
    unsigned *hist = calc_hist(in);
    fseek(in, 1 - terminal_mode, SEEK_SET);

#ifndef NDEBUG
    fprintf(stderr, "# FREQUENCY TABLE:\n");
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (hist[i])
        {
            fprintf(stderr, "%.2x - %u\n", i, hist[i]);
        }
    }
    fprintf(stderr, "# END FREQUENCY TABLE\n\n");
#endif

    ttree *root = create_tree(hist);
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

    if (!bit_stream || !hist || !root || !codes)
    {
        destroy_tree(root);
        free(bit_stream);
        free(codes);
        free(hist);
        return ENOMEM;
    }

    unsigned length = root->freq;
    if (fwrite(&length, sizeof(unsigned), 1, out) != 1)
    {
        destroy_tree(root);
        free(bit_stream);
        free(codes);
        free(hist);
        DEBUG_PRINT("Output error");
        return EIO;
    }

    if (pack_tree(root, bit_stream))
    {
        destroy_tree(root);
        free(bit_stream);
        free(codes);
        free(hist);
        return EIO;
    }

    unsigned char c = fgetc(in);
    while (!feof(in))
    {
        if (pack(c, codes, bit_stream))
        {
            free(bit_stream);
            destroy_tree(root);
            free(codes);
            free(hist);
            return EIO;
        }
        c = fgetc(in);
    }
    destroy_tree(root);
    free(codes);
    free(hist);
    if (flush(bit_stream))
    {
        free(bit_stream);
        return EIO;
    }
    free(bit_stream);
    return EXIT_SUCCESS;
}

int decode(FILE *in, FILE *out, char terminal_mode)
{
    if (fgetc(in) == EOF)
    {
        return EXIT_SUCCESS;
    }
    else
    {
        fseek(in, 1 - terminal_mode, SEEK_SET);
    }

    tbit_stream *bit_stream = create_bit_stream(in);
    if (!bit_stream)
    {
        return ENOMEM;
    }

    unsigned length;
    if (fread(&length, sizeof(unsigned), 1, in) != 1)
    {
        DEBUG_PRINT("Input error");
        return EIO;
    }

    int error_code = 0;
    ttree *root = unpack_tree(bit_stream, &error_code);
    if (!root)
    {
        free(bit_stream);
        return ENOMEM;
    }
    if (error_code)
    {
        destroy_tree(root);
        free(bit_stream);
        return error_code;
    }

    for (unsigned i = 0; i < length; i++)
    {
        unsigned char c;
        if (unpack(root, bit_stream, &c))
        {
            destroy_tree(root);
            free(bit_stream);
            return EIO;
        }
        if (fwrite(&c, sizeof(char), 1, out) != 1)
        {
            DEBUG_PRINT("Output error");
            destroy_tree(root);
            free(bit_stream);
            return EIO;
        }
    }

    destroy_tree(root);
    free(bit_stream);
    return EXIT_SUCCESS;
}
