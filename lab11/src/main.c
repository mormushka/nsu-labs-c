#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000

void printItems(const int n, int w, int *weight, int *value, int (*dp)[MAX_N + 1])
{
    int weight_r[n];
    int value_r[n];

    int j = w;
    printf("%d\n", dp[n][w]);

    for (int i = n; i > 0 && dp[i][j] > 0; --i)
    {
        if (dp[i][j] != dp[i - 1][j])
        {   
            weight_r[i - 1] = weight[i - 1];
            value_r[i - 1] = value[i - 1];
            j -= weight[i - 1];
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if ((weight_r[i] != 0) && (value_r[i] != 0))
        {
            printf("%d %d\n", weight_r[i], value_r[i]);
        }
    }
}

void knapsack(int n, int w, int *weight, int *value)
{
    int dp[n + 1][MAX_N + 1];

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
                int includeItem = value[i - 1] + dp[i - 1][j - weight[i - 1]];
                int excludeItem = dp[i - 1][j];
                if (includeItem > excludeItem)
                {
                    dp[i][j] = includeItem;
                }
                else
                {
                    dp[i][j] = excludeItem;
                }
            }
            else
            {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    printItems(n, w, weight, value, dp);
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

    int weight[n], value[n];
    for (int i = 0; i < n; ++i)
    {
        scanf("%d %d", &weight[i], &value[i]);
    }

    knapsack(n, w, weight, value);

    return 0;
}