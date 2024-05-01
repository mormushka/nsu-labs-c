#include <stdio.h>
#include "rb_tree.h"

int main()
{
    int count = 0;
    if (scanf("%d", &count) < 1)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    t_memory memory = create_memory(count, sizeof(rbt));
    if (memory.buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    rbt *root = input_tree(count, &memory);
    printf("%d", height(root, root != NULL));

    destroy_memory(&memory);

    return EXIT_SUCCESS;
}
