#include "code.h"
#include "debug_macros.h"
#include <stdlib.h>
#include <errno.h>

static void get_code(ttree *root, code *codes, char code[ALPHABET_SIZE], const int cur_len)
{
    if (is_leaf(root))
    {
        for (int i = 0; i < cur_len; ++i)
        {
            codes[root->symbol].code[i] = code[i];
        }
        codes[root->symbol].length = cur_len ? cur_len : (cur_len + 1);
        return;
    }
    code[cur_len] = 0;
    get_code(root->left, codes, code, cur_len + 1);
    code[cur_len] = 1;
    get_code(root->right, codes, code, cur_len + 1);
}

code *make_code_table(ttree *root)
{
    if (!root)
    {
        return NULL;
    }
    code *codes = calloc(sizeof(code), ALPHABET_SIZE);
    char *code = calloc(sizeof(char), ALPHABET_SIZE);
    if ((!codes) || (!code))
    {
        DEBUG_PRINT("Memory allocation failed");
        free(code);
        free(codes);
        return NULL;
    }
    get_code(root, codes, code, 0);
    free(code);
    return codes;
}

int pack(const int c, code *codes, tbit_stream *bit_stream)
{
    for (int i = 0; i < codes[c].length; ++i)
    {
        if (write_bit(codes[c].code[i], bit_stream))
        {
            DEBUG_PRINT("");
            return EIO;
        }
    }
    return EXIT_SUCCESS;
}
