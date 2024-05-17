#include <stdlib.h>
#include <errno.h>
#include "debug_macros.h"

enum
{
    WHITE,
    GRAY,
    BLACK,
    N_MAX = 2000,
};

typedef struct graph
{
    int n;
    int m;
    char *adj_matrix;
} graph;

int read_data(int *n, int *m)
{
    if (scanf("%d", n) < 1)
    {
        printf("bad number of lines");
        return EXIT_FAILURE;
    }
    if (*n < 0 || *n > N_MAX)
    {
        printf("bad number of vertices");
        return EXIT_FAILURE;
    }
    if (scanf("%d", m) < 1)
    {
        printf("bad number of lines");
        return EXIT_FAILURE;
    }
    int max = ((*n + 1) / 2) * (*n);
    if (*m < 0 || *m > max)
    {
        printf("bad number of edges");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int build_graph(graph *gr, int n, int m)
{
    gr->n = n;
    gr->m = m;
    int start, end;
    for (int i = 0; i < m; ++i)
    {
        if (scanf("%d %d", &start, &end) < 1)
        {
            printf("bad number of lines");
            return EXIT_FAILURE;
        }
        if (start < 0 || start > n || end < 0 || end > n)
        {
            printf("bad vertex");
            return EXIT_FAILURE;
        }
        *(gr->adj_matrix + start * (n + 1) + end) = 1;
    }
    return EXIT_SUCCESS;
}

int top_sort_dfs_visit(graph *gr, int v, int *color, int *top_sorted, int *i)
{
    color[v] = GRAY;

    for (int neightbour = 1; neightbour <= gr->n; ++neightbour)
    {
        if (*(gr->adj_matrix + v * (gr->n + 1) + neightbour) == 0)
        {
            continue;
        }
        if (color[neightbour] == GRAY)
        {
            return EXIT_FAILURE;
        }
        if (color[neightbour] == WHITE)
        {
            if (top_sort_dfs_visit(gr, neightbour, color, top_sorted, i))
            {
                return EXIT_FAILURE;
            }
        }
    }

    color[v] = BLACK;
    top_sorted[(*i)++] = v;
    return EXIT_SUCCESS;
}

int top_sort_dfs(graph *gr, int *top_sorted)
{
    int *color = calloc(sizeof(int), gr->n + 1);
    if (color == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }

    int i = 0;
    for (int v = 1; v <= gr->n; ++v)
    {
        if (color[v] == WHITE)
        {
            if (top_sort_dfs_visit(gr, v, color, top_sorted, &i))
            {
                free(color);
                return EXIT_FAILURE;
            }
        }
    }

    free(color);
    return EXIT_SUCCESS;
}

int main()
{
#if FREOPEN
    freopen("../in.txt", "r", stdin);
#endif
    int n, m;
    if (read_data(&n, &m))
    {
        return EXIT_SUCCESS;
    }

    graph *gr = malloc(sizeof(graph));
    if (gr == NULL)
    {
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }
    gr->adj_matrix = calloc(sizeof(char), (n + 1) * (n + 1));
    if (gr->adj_matrix == NULL)
    {
        free(gr);
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }

    if (build_graph(gr, n, m))
    {
        free(gr->adj_matrix);
        free(gr);
        return EXIT_SUCCESS;
    }

    int *top_sorted = calloc(sizeof(int), n);
    if (top_sorted == NULL)
    {
        free(gr->adj_matrix);
        free(gr);
        DEBUG_PRINT("Memory allocation failed");
        return ENOMEM;
    }

    int return_value = top_sort_dfs(gr, top_sorted);
    if (return_value == EXIT_FAILURE)
    {
        printf("impossible to sort");
        free(gr->adj_matrix);
        free(gr);
        free(top_sorted);
        return EXIT_SUCCESS;
    }
    else if (return_value) 
    {
        free(gr->adj_matrix);
        free(gr);
        free(top_sorted);
        return return_value;
    }

    for (int i = n - 1; i >= 0; --i)
    {
        printf("%d ", top_sorted[i]);
    }

    free(gr->adj_matrix);
    free(gr);
    free(top_sorted);
    return EXIT_SUCCESS;
}
