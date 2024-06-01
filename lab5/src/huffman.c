#include "huffman.h"
#include "code.h"
#include "bit_stream.h"
#include "tree.h"
#include "constants.h"
#include "debug_macros.h"
#include "progress_bar.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h>

void print_usage()
{
    printf("Usage: huffman.exe <command> <file.in> <file.out>\n");
    printf("<command> - symbol of command: c - compress, d - decompress\n");
    printf("<file.in> - input file\n");
    printf("<file.out> - output file\n");
}

static size_t *calc_hist(FILE *file)
{
    size_t *hist = calloc(ALPHABET_SIZE, sizeof(size_t));
    if (hist == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }

#ifdef ON_PROGRESS_BAR
    fprintf(stderr, "\n CALC_HIST: \n");
    size_t f_size = file_size(file);
    size_t cur_pos = ftell(file);
    size_t reload_p = f_size / 100;
    size_t p = 0;
#endif

#ifdef ON_DIF_TIME
    time_t start;
    time(&start);
#endif

    int curr_symbol = fgetc(file);
    while (curr_symbol != EOF)
    {
        hist[curr_symbol] += 1;
        curr_symbol = fgetc(file);
#ifdef ON_PROGRESS_BAR
        if (++p == reload_p)
        {
            print_progress((double)(cur_pos += p) / f_size);
            p = 0;
        }
#endif
    }

#ifdef ON_DIF_TIME
    time_t end;
    time(&end);
    fprintf(stderr, "\nCakl_hist took %.2lf seconds to run.\n", difftime(end, start));
#endif

    return hist;
}

static int math_len_shift(code *codes, size_t *hist)
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

static int shift(code *codes, size_t *hist, tbit_stream *bit_stream)
{
    int len_shift = math_len_shift(codes, hist);
    if (len_shift == 0)
    {
        for (int i = 1; i <= 8; ++i)
        {
            if (write_bit(i == 8, bit_stream))
            {
                DEBUG_PRINT("");
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
                DEBUG_PRINT("");
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
        if (codes[i].length)
        {
            for (int j = 0; j < codes[i].length; j++)
            {
                fprintf(stderr, "%d", codes[i].code[j]);
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
        DEBUG_PRINT("");
        return ENOMEM;
    }

    if (shift(codes, hist, bit_stream))
    {
        destroy_tree(root);
        free(bit_stream);
        free(codes);
        free(hist);
        DEBUG_PRINT("");
        return EIO;
    }

    if (pack_tree(root, bit_stream))
    {
        destroy_tree(root);
        free(bit_stream);
        free(codes);
        free(hist);
        DEBUG_PRINT("");
        return EIO;
    }

#ifdef ON_PROGRESS_BAR
    fprintf(stderr, "\n PACK: \n");
    size_t f_size = file_size(in);
    size_t cur_pos = ftell(in);
    size_t reload_p = f_size / 100;
    size_t p = 0;
#endif

#ifdef ON_DIF_TIME
    time_t start;
    time(&start);
#endif

    int c = fgetc(in);
    while (c != EOF)
    {
        if (pack(c, codes, bit_stream))
        {
            free(bit_stream);
            destroy_tree(root);
            free(codes);
            free(hist);
            DEBUG_PRINT("");
            return EIO;
        }
#ifdef ON_PROGRESS_BAR
        if (++p == reload_p)
        {
            print_progress((double)(cur_pos += p) / f_size);
            p = 0;
        }
#endif
        c = fgetc(in);
    }
#ifdef ON_PROGRESS_BAR
    print_progress((double)cur_pos / f_size);
#endif

#ifdef ON_DIF_TIME
    time_t end;
    time(&end);
    fprintf(stderr, "\nPack took %.2lf seconds to run.\n", difftime(end, start));
#endif

    destroy_tree(root);
    free(codes);
    free(hist);
    if (flush(bit_stream))
    {
        free(bit_stream);
        DEBUG_PRINT("");
        return EIO;
    }
    free(bit_stream);
    return EXIT_SUCCESS;
}

int decode(FILE *in, FILE *out, char terminal_mode)
{
#ifdef ON_DIF_TIME
    time_t start;
    time(&start);
#endif
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
        DEBUG_PRINT("");
        return ENOMEM;
    }

    int bit0 = 0;
    while (!bit0)
    {
        if (read_bit(bit_stream, &bit0))
        {
            free(bit_stream);
            DEBUG_PRINT("");
            return EIO;
        }
    }

    int error_code = 0;
    ttree *root = unpack_tree(bit_stream, &error_code);
    if (error_code)
    {
        destroy_tree(root);
        free(bit_stream);
        DEBUG_PRINT("");
        return error_code;
    }
#ifdef ON_PROGRESS_BAR
    fprintf(stderr, "\n UNPACK: \n");
    size_t f_size = file_size(in);
    size_t reload_p = file_size(in) / 40;
    size_t p = 0;
#endif

    for (;;)
    {
        unsigned char c;
        if (unpack(root, bit_stream, &c))
        {
            destroy_tree(root);
            free(bit_stream);
#ifdef ON_PROGRESS_BAR
            print_progress(((double)ftell(in)) / f_size);
#endif
#ifdef ON_DIF_TIME
            time_t end;
            time(&end);
            fprintf(stderr, "\nUnpack took %.2lf seconds to run.\n", difftime(end, start));
#endif
            return EXIT_SUCCESS;
        }
        if (fwrite(&c, sizeof(char), 1, out) != 1)
        {
            destroy_tree(root);
            free(bit_stream);
            DEBUG_PRINT("Output error");
            return EIO;
        }
#ifdef ON_PROGRESS_BAR
        if (++p == reload_p)
        {
            print_progress(((double)ftell(in)) / f_size);
            p = 0;
        }
#endif
    }
}
