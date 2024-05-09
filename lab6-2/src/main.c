#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "b-tree.h"
#include "debug-macros.h"

int main()
{
    //freopen("../in.txt", "r", stdin);
    int t;
    int count = 0;
    if (scanf("%d %d", &t, &count) < 2)
    {
        DEBUG_PRINT("Input error");
        return EIO;
    }

    b_tree *bt;
    int return_value = input_tree(&bt, t, count);
    if (return_value)
    {
        if (bt)
        {
            destroy_b_tree(bt);
        }
        return return_value;
    }

    printf("%d", height(bt));

    destroy_b_tree(bt);

    return EXIT_SUCCESS;
}
