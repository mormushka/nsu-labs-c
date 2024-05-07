#include <stdio.h>
#include <stdlib.h>

void print_items(const size_t n, const size_t w, size_t *weight, size_t *cost, size_t(*tc)[w + 1][2])
{
    printf("%zu\n", tc[n][w][0]);

    size_t j = w;
    int shopping_list[n + 1];
    for (size_t i = n; i > 0; --i)
    {
        if ((tc[i][j][0] != tc[i - 1][j][0]) || (tc[i][j][1] != tc[i - 1][j][1]))
        {
            shopping_list[i] = 1;
            j -= weight[i];
        }
        else
        {
            shopping_list[i] = 0;
        }
    }

    for (size_t i = 1; i <= n; ++i)
    {
        if (shopping_list[i] == 1)
        {
            printf("%zu %zu\n", weight[i], cost[i]);
        }
    }
}

void knapsack(const size_t n, const size_t w, size_t *weight, size_t *cost)
{
    size_t(*tc)[w + 1][2] = calloc((n + 1) * (w + 1) * 2, sizeof(size_t));
    if (tc == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return;
    }

    for (size_t i = 1; i <= n; ++i)
    {
        for (size_t j = 1; j <= w; ++j)
        {
            size_t previous_maximum = tc[i - 1][j][0];
            size_t w_previous_maximum = tc[i - 1][j][1];

            tc[i][j][0] = previous_maximum;
            tc[i][j][1] = w_previous_maximum;

            if (weight[i] <= j)
            {
                size_t cost_with_free_space = cost[i] + tc[i - 1][j - weight[i]][0];
                size_t weight_with_free_space = weight[i] + tc[i - 1][j - weight[i]][1];

                if (cost_with_free_space > previous_maximum)
                {
                    tc[i][j][0] = cost_with_free_space;
                    tc[i][j][1] = weight_with_free_space;
                }
                if (cost_with_free_space == previous_maximum)
                {
                    if (weight_with_free_space > w_previous_maximum)
                    {
                        tc[i][j][1] = w_previous_maximum;
                    }
                    else
                    {
                        tc[i][j][1] = weight_with_free_space;
                    }
                }
            }
        }
    }

    print_items(n, w, weight, cost, tc);
    free(tc);
}

int main()
{
    size_t n, w;
    if (scanf("%zu %zu", &n, &w) < 2)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    size_t weight[n + 1], cost[n + 1];
    weight[0] = 0;
    cost[0] = 0;
    for (size_t i = 1; i <= n; ++i)
    {
        if (scanf("%zu %zu", &weight[i], &cost[i]) < 2)
        {
            fprintf(stderr, "Input error %d\n", __LINE__);
            return EXIT_FAILURE;
        }
    }

    knapsack(n, w, weight, cost);

    return EXIT_SUCCESS;
}