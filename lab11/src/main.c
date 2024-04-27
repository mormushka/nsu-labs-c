#include <stdio.h>
#include <stdlib.h>

size_t max(size_t a, size_t b)
{
    return (a > b) ? a : b;
}

void print_items(const size_t n, const size_t w, size_t *weight, size_t *cost, void *tc_void)
{
    size_t (*tc)[w + 1] = tc_void;
    size_t *r_weight = calloc(n + 1, sizeof(size_t));
    if (r_weight == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return;
    }
    size_t *r_cost = calloc(n + 1, sizeof(size_t));
    if (r_cost == NULL)
    {
        free(r_weight);
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return;
    }

    printf("%zu\n", tc[n][w]);

    size_t j = w;
    for (size_t i = n; i > 0; --i)
    {
        if (tc[i][j] != tc[i - 1][j])
        {   
            r_weight[i] = weight[i];
            r_cost[i] = cost[i];
            j -= weight[i];
        }
    }

    for (size_t i = 1; i <= n; ++i)
    {
        if (r_weight[i] != 0)
        {
            printf("%zu %zu\n", r_weight[i], r_cost[i]);
        }
    }

    free(r_weight);
    free(r_cost);
}

void knapsack(const size_t n, const size_t w, size_t *weight, size_t *cost)
{
    size_t (*tc)[w + 1] = calloc( (n + 1) * (w + 1), sizeof(size_t));
    if (tc == NULL)
    {
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return;
    }


    for (size_t i = 1; i <= n; ++i)
    {
        for (size_t j = 1; j <= w; ++j)
        {
            if (weight[i] <= j)
            {
                size_t previous_maximum = tc[i - 1][j];
                size_t cost_with_free_space = cost[i] + tc[i - 1][j - weight[i]];
                tc[i][j] =  max(previous_maximum, cost_with_free_space);
            }
            else
            {
                tc[i][j] = tc[i - 1][j];
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