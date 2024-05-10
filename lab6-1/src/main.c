#include <stdio.h>
#include <errno.h>
#include "rb_tree.h"
#include "debug_macros.h"

int main()
{
    int count = 0;
    if (scanf("%d", &count) < 1)
    {
        DEBUG_PRINT("Input error");
        return EIO;
    }

    t_memory memory = create_memory(count, sizeof(rbt));
    if (memory.buffer == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }

    rbt *root;
    int return_value = input_tree(&root, count, &memory);
    if (return_value) 
    {
        destroy_memory(&memory);
        return return_value;
    }

    printf("%d", height(root, root != NULL));

    destroy_memory(&memory);

    return EXIT_SUCCESS;
}
