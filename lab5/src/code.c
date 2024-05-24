#include "code.h"

void get_codes(tree_node *root, code *codes, const unsigned long long code, const int cur_len)
{
    if (is_leaf(root))
    {
        codes[root->symbol].code = code;
        codes[root->symbol].length = cur_len;
        return;
    }
    get_codes(root->left, codes, code << 1, cur_len + 1);
    get_codes(root->right, codes, (code << 1) | 1, cur_len + 1);
}

code * make_code_table(tree_node *root)
{
    if (!root)
    {
        return NULL;
    }
    code *codes = malloc(sizeof(struct code) * ALPHABET_SIZE);
    if (codes == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return NULL;
    }
    get_codes(root, codes, 0, 0);
    return codes;
}

int pack(const unsigned char c, code *codes, bit_stream *stream)
{
    for (int i = codes[c].length - 1; i >= 0; i--)
    {
        int bit = (codes[c].code >> i) & 1;
        if (write_bit(bit, stream))
        {
            return EIO;
        }
    }
    return EXIT_SUCCESS;
}
