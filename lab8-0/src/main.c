#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define N_MAX 5000

typedef struct t_item
{
    int start;
    int end;
    int weight;
} t_item;

void fill_item(int start, int end, int weight, t_item *item)
{
    item->end = end;
    item->start = start;
    item->weight = weight;
}

int read_data(int *n, int *m)
{
    if (scanf("%d %d", n, m) < 2)
    {
        printf("bad number of lines\n");
        return EXIT_FAILURE;
    }
    if ((*n < 0) || (*n > N_MAX))
    {
        printf("bad number of vertices\n");
        return EXIT_FAILURE;
    }
    if ((*m < 0) || (*m > ((*n + 1) / 2) * (*n)))
    {
        printf("bad number of edges\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int build_graph(int n, int m, t_item *items)
{
    int start, end;
    long long int weight;
    for (int i = 0; i < m; ++i)
    {
        if (scanf("%d %d %lld", &start, &end, &weight) < 1)
        {
            printf("bad number of lines\n");
            return EXIT_FAILURE;
        }
        if ((start < 0) || (start > n) || (end < 0) || (end > n))
        {
            printf("bad vertex\n");
            return EXIT_FAILURE;
        }
        if ((weight > INT_MAX) || (weight < 0))
        {
            printf("bad length\n");
            return EXIT_FAILURE;
        }
        fill_item(start, end, (int)weight, &items[i]);
    }
    return EXIT_SUCCESS;
}

int find_set(int *parent, int x)
{
    return (x == parent[x]) ? x : (parent[x] = find_set(parent, parent[x]));
}

void union_set(int x, int y, int *parent)
{
    parent[find_set(parent, x)] = find_set(parent, y);
}

int compare(const void *av, const void *bv)
{
    return ((t_item *)av)->weight - ((t_item *)bv)->weight;
}

int check_mst(char *marker, int n)
{
    for (int i = 0; i < n; ++i)
    {
        if (marker[i])
        {
            return 1;
        }
    }
    return 0;
}

int kruskal(t_item *items, int n, int m)
{
    int parent[n];
    char marker[n];
    for (int i = 0; i < n; ++i)
    {
        parent[i] = i;
        marker[i] = 1;
    }

    qsort(items, m, sizeof(*items), compare);

    int j = 0;
    for (int i = 0; i < m; ++i)
    {
        if (find_set(parent, items[i].start - 1) != find_set(parent, items[i].end - 1))
        {
            union_set(items[i].start - 1, items[i].end - 1, parent);
            items[j].start = items[i].start;
            items[j].end = items[i].end;
            marker[items[i].start - 1] = 0;
            marker[items[i].end - 1] = 0;
            j++;
        }
    }

    if ((n == 0) || (check_mst(marker, n) && (n != 1)))
    {
        return -1;
    }

    return j;
}

int main()
{
    int n, m;
    if (read_data(&n, &m))
    {
        return EXIT_SUCCESS;
    }

    t_item *items = malloc(sizeof(t_item) * m);
    if (!items)
    {
        return ENOMEM;
    }
    if (build_graph(n, m, items))
    {
        free(items);
        return EXIT_SUCCESS;
    }

    int len_mst = kruskal(items, n, m);

    if (len_mst == -1)
    {
        printf("no spanning tree\n");
        free(items);
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < len_mst; ++i)
    {
        if (items[i].start > items[i].end)
        {
            printf("%d %d\n", items[i].end, items[i].start);
        }
        else
        {
            printf("%d %d\n", items[i].start, items[i].end);
        }
    }

    free(items);
    return EXIT_SUCCESS;
}
