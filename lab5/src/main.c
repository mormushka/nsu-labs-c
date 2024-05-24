#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    ALPHABET = 256,
    SPECIAL = 257,
    CHAR_SIZE = 8
};

typedef struct byte_context
{
    int file_size;
    int last_byte;
    int current_size;
    int cursor;
    char buffer;
} byte_context;

typedef struct node_tree
{
    int letter;
    int priority;
    struct node_tree *left;
    struct node_tree *right;
    struct node_tree *next;
} node_tree;

typedef struct linked_list
{
    node_tree *head;
    int len_of_list;
} linked_list;

void print_usage()
{
    printf("Usage: huffman.exe <command> <file.in> <file.out>\n");
    printf("<command> - symbol of command: c - compress, d - decompress\n");
    printf("<file.in> - input file\n");
    printf("<file.out> - output file\n");
}

node_tree *create_start_node_tree(int chr, int prior, node_tree *l, node_tree *r)
{
    node_tree *node = (node_tree *)malloc(sizeof(node_tree));
    node->letter = chr;
    node->priority = prior;
    node->left = l;
    node->right = r;
    node->next = NULL;
    return node;
}

linked_list *create_list()
{
    linked_list *list = (linked_list *)malloc(sizeof(linked_list));
    list->head = NULL;
    list->len_of_list = 0;
    return list;
}

void init_list(linked_list *list, node_tree *data)
{
    list->head = data;
    list->len_of_list++;
}

void add_node(linked_list *list, node_tree *cur_node, node_tree *data)
{
    if (cur_node->next != NULL)
    {
        data->next = cur_node->next;
    }
    cur_node->next = data;
    list->len_of_list++;
}

void delete_node(linked_list *list, node_tree *cur_node, int head_indicator)
{
    if (head_indicator)
    {
        list->head = cur_node->next;
    }
    else
    {
        node_tree *tmp = cur_node->next->next;
        cur_node->next = tmp;
    }
    list->len_of_list--;
}

void push_front(linked_list *list, node_tree *data)
{
    node_tree *cur_node = list->head;
    if (data->priority <= list->head->priority)
    {
        add_node(list, cur_node, data);
    }
    else
    {
        for (int i = 1; i < list->len_of_list; ++i)
        {
            if (data->priority <= cur_node->next->priority)
            {
                break;
            }
            cur_node = cur_node->next;
        }
        add_node(list, cur_node, data);
    }
}

int find_node(linked_list *list, unsigned char letter, node_tree **cur_node)
{
    for (int i = 1; i < list->len_of_list; ++i)
    {
        if ((*cur_node)->next->letter == letter)
        {
            return 1;
        }
        (*cur_node) = (*cur_node)->next;
    }
    return 0;
}

#if 0
void change_priority(linked_list *list, unsigned char letter)
{
    node_tree *cur_node = list->head;
    node_tree *editable_node = cur_node;
    int founded_flag = 1;
    if (cur_node->letter != letter)
    {
        founded_flag = find_node(list, letter, &cur_node);
        editable_node = cur_node->next;
    }
    if (founded_flag)
    {
        delete_node(list, cur_node, editable_node == list->head);
        editable_node->priority += 1;
    }
    else
    {
        editable_node = create_start_node_tree(letter, 1, NULL, NULL);
    }
    if (list->len_of_list == 0)
    {
        init_list(list, editable_node);
    }
    else
    {
        push_front(list, editable_node);
    }
}
#endif

node_tree *pop(linked_list *list)
{
    if (list->len_of_list != 0)
    {
        node_tree *out = list->head;
        list->head = out->next;
        list->len_of_list--;
        return out;
    }
    return NULL;
}

int is_empty(linked_list *list)
{
    if (list->len_of_list == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void garbage_collector(node_tree *node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->left != NULL)
    {
        garbage_collector(node->left);
    }
    if (node->right != NULL)
    {
        garbage_collector(node->right);
    }
    free(node);
}

void delete_list(linked_list *list)
{
    for (int i = 0; i < list->len_of_list; ++i)
    {
        node_tree *prev = list->head->next;
        garbage_collector(list->head);
        list->head = prev;
    }
    free(list);
}

linked_list *create_priority_queue(FILE *fl)
{
    linked_list *list = create_list();
    unsigned char current_char;
    int count_char = 0;
    int frequency[ALPHABET] = {0};

    while (fread(&current_char, 1, 1, fl) == 1)
    {
        frequency[current_char]++;
        count_char++;
    }

    if (count_char == 0)
    {
        delete_list(list);
        return NULL;
    }

    for (int i = 0; i < ALPHABET; i++)
    {
        if (frequency[i] > 0)
        {
            node_tree *node = create_start_node_tree(i, frequency[i], NULL, NULL);
            if (is_empty(list))
            {
                init_list(list, node);
            }
            else
            {
                push_front(list, node);
            }
        }
    }

    return list;
}

void create_tree(linked_list *list)
{
    while (list->len_of_list != 1)
    {
        node_tree *left = pop(list);
        node_tree *right = pop(list);
        node_tree *node = create_start_node_tree(SPECIAL, left->priority + right->priority, left, right);
        if (list->len_of_list == 0)
        {
            init_list(list, node);
        }
        else
        {
            push_front(list, node);
        }
    }
}

/* ENCODING */
char *get_letter_code(char letter)
{
    char *string_letter = (char *)malloc(ALPHABET);
    for (int i = 0; i < CHAR_SIZE; i++)
    {
        string_letter[i] = (letter & 1) ? '1' : '0';
        letter = (char)(letter >> 1);
    }
    string_letter[CHAR_SIZE] = '\0';
    return string_letter;
}

void binary_write(FILE *fl_out, const char *code, byte_context *context)
{
    int index = 0;
    while (code[index] != '\0')
    {
        context->buffer = (char)(((context->buffer) << 1) | (code[index] == '1' ? 1 : 0));
        index++;
        context->cursor += 1;
        if (context->cursor == CHAR_SIZE)
        {
            char c = 0;
            for (int i = 0; i < CHAR_SIZE; ++i)
            {
                c = (char)((c << 1) | (context->buffer & 1));
                context->buffer = (char)(context->buffer >> 1);
            }
            fwrite(&c, 1, 1, fl_out);
            context->cursor = 0;
        }
    }
}

void encode_tree(node_tree *root, char code[], char array_of_code[ALPHABET][ALPHABET], byte_context *context, int index, FILE *fl_out)
{
    if (root == NULL)
    {
        return;
    }
    if (root->letter != SPECIAL)
    {
        binary_write(fl_out, "1", context);
        char *reverse_letter = get_letter_code(root->letter);
        binary_write(fl_out, reverse_letter, context);
        free(reverse_letter);
        if (code[0] == '\0')
        {
            code[0] = '0';
        }
        strcpy(array_of_code[root->letter], code);
    }
    else
    {
        binary_write(fl_out, "0", context);
    }

    code[index] = '0';
    code[index + 1] = '\0';
    encode_tree(root->left, code, array_of_code, context, index + 1, fl_out);
    code[index] = '1';
    code[index + 1] = '\0';
    encode_tree(root->right, code, array_of_code, context, index + 1, fl_out);
}

void write_last_byte(FILE *fl_out, byte_context *context)
{
    int number_of_bits_left = context->cursor;
    while (context->cursor != CHAR_SIZE)
    {
        context->buffer = context->buffer << 1;
        context->cursor++;
    }
    char c = 0;
    for (int i = 0; i < CHAR_SIZE; ++i)
    {
        c = (char)((c << 1) | (context->buffer & 1));
        context->buffer = (context->buffer >> 1);
    }
    fwrite(&c, 1, 1, fl_out);
    c = (char)(number_of_bits_left + '0');
    fwrite(&c, 1, 1, fl_out);
}

void encode(FILE *fl, FILE *fl_out, char terminal_mode)
{
    linked_list *list = create_priority_queue(fl);
    if (list == NULL)
    {
        return;
    }
    create_tree(list);

    byte_context *context = (byte_context *)malloc(sizeof(byte_context));
    context->buffer = 0;
    context->cursor = 0;
    char array_of_code[ALPHABET][ALPHABET];
    char code[ALPHABET];
    for (int i = 0; i < ALPHABET; ++i)
    {
        code[i] = '\0';
    }
    encode_tree(list->head, code, array_of_code, context, 0, fl_out);
    /* WRITE TEXT AS CODE */
    unsigned char current_char;
    fseek(fl, 1 - terminal_mode, SEEK_SET);
    while (fread(&current_char, 1, 1, fl) == 1)
    {
        binary_write(fl_out, array_of_code[current_char], context);
    }
    /* IF WE HAVE CHARS, WRITE WITH FLAG */
    write_last_byte(fl_out, context);
    delete_list(list);
    free(context);
}

/* DECODING */
void check_shift(FILE *fl, char *current_char, byte_context *context)
{
    if (context->cursor >= CHAR_SIZE)
    {
        context->cursor = 0;
        context->current_size++;
        if (fread(current_char, 1, 1, fl) < 1)
        {
            return;
        }
    }
}

char create_letter_from_code(FILE *fl, char *current_char, byte_context *context)
{
    char chr = 0;
    for (int i = 0; i < CHAR_SIZE; ++i)
    {
        check_shift(fl, current_char, context);
        chr = (char)((chr << 1) | (*current_char & 1));
        *current_char = (char)(*current_char >> 1);
        context->cursor++;
    }
    char letter = 0;
    for (int i = 0; i < 8; ++i)
    {
        letter = (char)((letter << 1) | (chr & 1));
        chr = (char)(chr >> 1);
    }
    return letter;
}

node_tree *build_tree(FILE *fl, char *current_char, byte_context *context)
{
    if (*current_char & 1)
    {
        *current_char = (char)(*current_char >> 1);
        context->cursor++;
        return create_start_node_tree(create_letter_from_code(fl, current_char, context), 0, NULL, NULL);
    }
    *current_char = (char)(*current_char >> 1);
    context->cursor++;
    node_tree *node = create_start_node_tree(SPECIAL, 0, NULL, NULL); ///
    check_shift(fl, current_char, context);
    node->left = build_tree(fl, current_char, context); ///
    check_shift(fl, current_char, context);
    node->right = build_tree(fl, current_char, context);
    return node;
}

node_tree *tree_shift(node_tree *node, int direction)
{
    if (direction && node->right != NULL)
    {
        return node->right;
    }
    else if (node->left != NULL)
    {
        return node->left;
    }
    return node;
}

void decode_text(FILE *fl, FILE *fl_out, byte_context *context)
{
    char current_char;
    if (fread(&current_char, 1, 1, fl) < 1)
    {
        return;
    }
    context->cursor = 0;
    context->current_size = 2;
    node_tree *root = build_tree(fl, &current_char, context);
    node_tree *current_node = root;
    while (context->current_size < context->file_size ||
           (context->current_size >= context->file_size && context->cursor < context->last_byte) != 0)
    {
        current_node = tree_shift(current_node, current_char & 1);
        /* IF WE FIND CHAR - WRITE IT */
        if (current_node->left == NULL && current_node->right == NULL)
        {
            fwrite(&current_node->letter, 1, 1, fl_out);
            current_node = root;
        }
        current_char = (char)(current_char >> 1);
        context->cursor++;
        check_shift(fl, &current_char, context);
    }
    garbage_collector(root);
}

void decode(FILE *fl, FILE *fl_out, char terminal_mode)
{
    char current_char;
    fseek(fl, -1, SEEK_END);
    if (fread(&current_char, 1, 1, fl) < 1)
    {
        return;
    }
    byte_context *context = (byte_context *)malloc(sizeof(byte_context));
    context->last_byte = current_char - '0';
    context->file_size = ftell(fl) - 1 + terminal_mode;
    fseek(fl, 1 - terminal_mode, SEEK_SET);
    decode_text(fl, fl_out, context);
    free(context);
}

int main(int argc, char *argv[])
{
    if (argc == 4)
    {
        char mode = argv[1][0];
        if (mode != 'c' && mode != 'd')
        {
            print_usage();
            return EXIT_FAILURE;
        }

        FILE *fl = fopen(argv[2], "rb");
        if (fl == NULL)
        {
            printf("Error: Could not open input file %s for reading.\n", argv[2]);
            print_usage();
            return EXIT_FAILURE;
        }

        FILE *fl_out = fopen(argv[3], "wb");
        if (fl_out == NULL)
        {
            printf("Error: Could not open output file %s for writing.\n", argv[3]);
            fclose(fl);
            print_usage();
            return EXIT_FAILURE;
        }

        if (mode == 'c')
        {
            encode(fl, fl_out, 1);
        }
        else if (mode == 'd')
        {
            decode(fl, fl_out, 1);
        }

        fclose(fl);
        fclose(fl_out);
    }
    else if (argc == 1)
    {
        char mode;
        FILE *fl = fopen("in.txt", "rb");
        FILE *flOut = fopen("out.txt", "wb");
        if (fread(&mode, 1, 1, fl) < 1)
        {
            fclose(fl);
            fclose(flOut);
            return EXIT_FAILURE;
        }
        if (mode == 'c')
        {
            encode(fl, flOut, 0);
        }
        else
        {
            decode(fl, flOut, 0);
        }
        fclose(fl);
        fclose(flOut);
        return EXIT_SUCCESS;
    }
    else
    {
        print_usage();
        return EXIT_FAILURE;
    }
}