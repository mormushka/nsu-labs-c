#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int read_data(int *n, int *m)
{
    if (scanf("%d %d", n, m) < 2)
    {
        printf("bad number of lines");
        return EXIT_FAILURE;
    }
    if ((*n < 0) || (*n > 5000))
    {
        printf("bad number of vertices");
        return EXIT_FAILURE;
    }
    if ((*m < 0) || (*m > ((*n) * ((*n) + 1)) / 2))
    {
        printf("bad number of edges");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int build_graph(int n, int m, unsigned (*graph)[n])
{
    int start, end;
    unsigned long long int weight;
    for (int i = 0; i < m; ++i)
    {
        if (scanf("%d %d %llu", &start, &end, &weight) < 1)
        {
            printf("bad number of lines\n");
            return EXIT_FAILURE;
        }
        if ((start < 0) || (start > n) || (end < 0) || (end > n))
        {
            printf("bad vertex\n");
            return EXIT_FAILURE;
        }
        if (weight > INT_MAX)
        {
            printf("bad length\n");
            return EXIT_FAILURE;
        }
        graph[start - 1][end - 1] = weight;
        graph[end - 1][start - 1] = weight;
    }
    return EXIT_SUCCESS;
}

int find_lovest_weight_node(unsigned *weight, char *processed, int n)
{
    int min_node = 0;
    unsigned int min_weight = UINT_MAX;
    for (int i = 0; i < n; i++)
    {
        if ((!processed[i]) && (weight[i] < min_weight))
        {
            min_weight = weight[i];
            min_node = i + 1;
        }
    }
    return min_node;
}

int prim(int *parent, int n, unsigned (*graph)[n])
{
    if (n == 0)
    {
        printf("no spanning tree");
        return EXIT_FAILURE;
    }

    char processed[n];
    unsigned weight[n];
    for (int i = 0; i < n; i++)
    {
        processed[i] = 0;
        weight[i] = UINT_MAX;
    }

    weight[0] = 0;
    for (int i = 0; i < n; i++)
    {
        int node = find_lovest_weight_node(weight, processed, n);
        if (!node)
        {
            printf("no spanning tree");
            return EXIT_FAILURE;
        }
        
        processed[node - 1] = 1;
        for (int j = 1; j < n; j++)
        {
            if ((!processed[j]) && (graph[node - 1][j]) && (graph[node - 1][j] < weight[j]))
            {
                parent[j] = node;
                weight[j] = graph[node - 1][j];
            }
        }
    }
    return EXIT_SUCCESS;
}

int main()
{
    int n, m;
    if (read_data(&n, &m))
    {
        return EXIT_SUCCESS;
    }
    unsigned(*graph)[n] = calloc(n * n, sizeof(int));
    if (!graph)
    {
        return ENOMEM;
    }

    if (build_graph(n, m, graph))
    {
        free(graph);
        return EXIT_SUCCESS;
    }

    int parent[n];
    if (prim(parent, n, graph)) 
    {
        free(graph);
        return EXIT_SUCCESS;
    }
    for (int i = 1; i < n; i++)
    {
        printf("%d %d\n", parent[i], i + 1);
    }
    free(graph);
    return EXIT_SUCCESS;
}
