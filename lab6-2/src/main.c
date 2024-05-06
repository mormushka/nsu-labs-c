#include <stdlib.h>
#include <stdio.h>
#include "b-tree.h"

int main()
{
    int t;
    int count = 0;
    if (scanf("%d %d", &t, &count) < 2)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    input_tree_return_form return_form = input_tree(t, count);
    if (return_form.error_code == EXIT_FAILURE)
    {
        if (return_form.bt)
        {
            destroy_b_tree(return_form.bt);
        }
        return EXIT_FAILURE;
    }

    b_tree *bt = return_form.bt;

    printf("%d", height(bt));

    destroy_b_tree(bt);

    return EXIT_SUCCESS;
}
