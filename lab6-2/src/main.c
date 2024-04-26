#include <stdlib.h>
#include <stdio.h>
#include "b-tree.h"

int main()
{
    int t = 2;
    int count = 0;
    if (scanf("%d %d", &t, &count) < 2)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    b_tree *bt = input_tree(t, count);

    printf("%d", height(bt));

    destroy_b_tree(bt);

    return EXIT_SUCCESS;
}
