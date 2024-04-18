#include <stdio.h>
#include <stdlib.h>

void printItems(const int n, const int w, int *weight, int  *cost, void *dpp)
{
    int (*dp)[w + 1] = dpp;
    int *r_weight = calloc(n, sizeof(int));
    int *r_cost = calloc(n, sizeof(int));

    printf("%d\n", dp[n][w]);

    int j = w;
    for (int i = n; i > 0; --i)
    {
        if (dp[i][j] != dp[i - 1][j])
        {   
            r_weight[i - 1] = weight[i - 1];
            r_cost[i - 1] = cost[i - 1];
            j -= weight[i - 1];
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if ((r_weight[i] != 0) && (r_cost[i] != 0))
        {
            printf("%d %d\n", r_weight[i], r_cost[i]);
        }
    }
}

void knapsack(const int n, const int w, int *weight, int *cost)
{
    int dp[n + 1][w + 1];

    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= w; ++j)
        {
            if ((i == 0) || (j == 0))
            {
                dp[i][j] = 0;
            }
            else if (weight[i - 1] <= j)
            {
                int previous_maximum = dp[i - 1][j];
                int cost_with_free_space = cost[i - 1] + dp[i - 1][j - weight[i - 1]];
                dp[i][j] = (previous_maximum > cost_with_free_space) ? previous_maximum : cost_with_free_space;
            }
            else
            {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    printItems(n, w, weight, cost, dp);
}


int main()
{
    //FILE *in = freopen("../in.txt", "r", stdin);
    int n, w;
    if (scanf("%d %d", &n, &w) < 2)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    int weight[n], cost[n];
    for (int i = 0; i < n; ++i)
    {
        scanf("%d %d", &weight[i], &cost[i]);
    }

    knapsack(n, w, weight, cost);

    return 0;
}