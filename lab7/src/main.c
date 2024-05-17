#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include "debug_macros.h"

enum
{
    WHITE, /* вершина, из которой еще не был запущен dfs_visit */
    GRAY,  /* вершина, для которой dfs_visit не завершился */
    BLACK, /* вершина, для которой dfs_visit завершен */
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

bool top_sort_dfs_visit(graph *gr, int v, int *color, int *top_sorted, int *i)
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
            return false;
        }
        if (color[neightbour] == WHITE)
        {
            if (!top_sort_dfs_visit(gr, neightbour, color, top_sorted, i))
            {
                return false;
            }
        }
    }

    color[v] = BLACK;
    top_sorted[(*i)++] = v;
    return true;
}

bool top_sort_dfs(graph *gr, int *top_sorted)
{
    int *color = calloc(sizeof(int), gr->n + 1);
    // calloc

    int i = 0;
    for (int v = 1; v <= gr->n; ++v)
    {
        if (color[v] == WHITE)
        {
            if (!top_sort_dfs_visit(gr, v, color, top_sorted, &i))
            {
                free(color);
                return false;
            }
        }
    }

    free(color);
    return true;
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

    graph *gr = malloc(sizeof(gr));
    if (gr == NULL)
    {
        return ENOMEM;
    }
    gr->adj_matrix = calloc(sizeof(char), (n + 1) * (n + 1));
    if (gr->adj_matrix == NULL)
    {
        free(gr);
        return ENOMEM;
    }

    int return_value = build_graph(gr, n, m);
    if (return_value)
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
        return ENOMEM;
    }
    if (!top_sort_dfs(gr, top_sorted))
    {
        printf("impossible to sort");
        free(gr->adj_matrix);
        free(gr);
        free(top_sorted);
        return EXIT_SUCCESS;
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
