#include "huffman.h"

void print_usage()
{
    printf("Usage: huffman.exe <command> <file.in> <file.out>\n");
    printf("<command> - symbol of command: c - compress, d - decompress\n");
    printf("<file.in> - input file\n");
    printf("<file.out> - output file\n");
}

size_t *calc_hist(FILE *file)
{
    size_t *hist = calloc(ALPHABET_SIZE, sizeof(size_t));
    if (hist == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    int curr_symbol = fgetc(file);
    while (curr_symbol != EOF)
    {
        hist[curr_symbol] += 1;
        curr_symbol = fgetc(file);
    }

    return hist;
}

int math_len_shift(code *codes, size_t *hist)
{
    int len = 0;
    int n = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i)
    {
        if (hist[i])
        {
            len = (len + codes[i].length * hist[i]) % 8;
            n++;
        }
    }
    return 8 - ((len + 2 * n - 1) % 8);
}

int shift(code *codes, size_t *hist, tbit_stream *bit_stream)
{
    int len_shift = math_len_shift(codes, hist);
    if (len_shift == 0)
    {
        for (int i = 1; i <= 8; ++i)
        {
            if (write_bit(i == 8, bit_stream))
            {
                return EIO;
            }
        }
    }
    else
    {
        for (int i = 1; i <= len_shift; ++i)
        {
            if (write_bit(i == len_shift, bit_stream))
            {
                return EIO;
            }
        }
    }
    return EXIT_SUCCESS;
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
    size_t *hist = calc_hist(in);
    fseek(in, 1 - terminal_mode, SEEK_SET);

#ifndef NDEBUG
    fprintf(stderr, "# FREQUENCY TABLE:\n");
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (hist[i])
        {
            fprintf(stderr, "%.2x - %zu\n", i, hist[i]);
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

#if 0
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
#endif

    if (shift(codes, hist, bit_stream))
    {
        destroy_tree(root);
        free(bit_stream);
        free(codes);
        free(hist);
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

    int c = fgetc(in);
    while (c != EOF)
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

#if 0
    unsigned length;
    if (fread(&length, sizeof(unsigned), 1, in) != 1)
    {
        DEBUG_PRINT("Input error");
        return EIO;
    }
#endif
    int bit0 = 0;
    while (!bit0)
    {
        if (read_bit(bit_stream, &bit0))
        {
            free(bit_stream);
            return EIO;
        }
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

    for(;;)
    {
        unsigned char c;
        if (unpack(root, bit_stream, &c))
        {
            destroy_tree(root);
            free(bit_stream);
            return EXIT_SUCCESS;
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
