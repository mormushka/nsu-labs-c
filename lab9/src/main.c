#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>


#define DEBUG1

typedef struct path
{
    uint64_t *costs;
    int *parents;
} path;

typedef struct dijkstra_protocol
{
    uint64_t *costs;
    int *parents;
} dijkstra_protocol;

int is_bad_vertex(const unsigned v, const unsigned N)
{
    return (v == 0) || (v > N);
}

int input_line(unsigned *s_edge, unsigned *f_edge, uint64_t *len_edge, const unsigned N)
{
    int a = scanf("%u %u %lu", s_edge, f_edge, len_edge);
    if (a == EOF)
    {
        printf("bad number of lines");
        return 0;
    }

    if (a < 3)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return 0;
    }

    if (is_bad_vertex(*s_edge, N) || is_bad_vertex(*f_edge, N))
    {
        printf("bad vertex");
        return 0;
    }

    if ((*len_edge == 0) || (*len_edge > INT_MAX))
    {
        printf("bad length");
        return 0;
    }

    return 1;
}

void *input_graph(const unsigned N, const unsigned S, const unsigned F, const unsigned M)
{
    if (N > 5000)
    {
        printf("bad number of vertices");
        return NULL;
    }

    if (is_bad_vertex(S, N) || is_bad_vertex(F, N))
    {
        printf("bad vertex");
        return NULL;
    }

    if (M > N * (N + 1) / 2)
    {
        printf("bad number of edges");
        return NULL;
    }

    uint64_t(*adjacency_matrix)[N + 1] = calloc((N + 1) * (N + 1), sizeof(uint64_t));

    for (int i = 0; i < M; ++i)
    {
        unsigned s_edge, f_edge;
        uint64_t len_edge;

        if (!input_line(&s_edge, &f_edge, &len_edge, N))
        {
            free(adjacency_matrix);
            return NULL;
        }

        adjacency_matrix[s_edge][f_edge] = len_edge;
        adjacency_matrix[f_edge][s_edge] = len_edge;
    }

    return adjacency_matrix;
}

int find_lovest_cost_node(uint64_t *costs, char *processed, const int N)
{
    int lovest_cost_node = 0;
    uint64_t lovest_cost = UINT64_MAX;

    for (int i = 1; i <= N; ++i)
    {
        if ((!processed[i]) && (costs[i] < lovest_cost))
        {
            lovest_cost = costs[i];
            lovest_cost_node = i;
        }
    }

    return lovest_cost_node;
}

void init_costs(uint64_t* costs, const int N)
{
    for (int i = 1; i <= N; ++i)
    {
        costs[i] = UINT64_MAX;
    }
}

dijkstra_protocol *dijkstra_algorithm(const unsigned S, void *a_matrix, const unsigned N)
{
    uint64_t(*adjacency_matrix)[N + 1] = a_matrix;

    uint64_t *costs = calloc(N + 1, sizeof(uint64_t));
    int *parents = calloc(N + 1, sizeof(int));
    char *processed = calloc(N + 1, sizeof(char));
    if ((costs == NULL) || (parents == NULL) || (processed == NULL))
    {
        free(costs);
        free(parents);
        free(processed);
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return NULL;
    }
    
    init_costs(costs, N);
    costs[S] = 0;

    int node = find_lovest_cost_node(costs, processed, N);
    while (node)
    {
        uint64_t cost_node = costs[node];
        for (int i = 1; i <= N; ++i)
        {
            if (adjacency_matrix[node][i])
            {
                uint64_t new_cost = cost_node + adjacency_matrix[node][i];
                if (new_cost < costs[i])
                {
                    costs[i]= new_cost;
                    parents[i] = node;
                }
            }
        }
        processed[node] = 1;
        node = find_lovest_cost_node(costs, processed, N);
    }

#ifdef DEBUG
    printf("costs:     ");
    for (int i = 1; i <= N; ++i)
    {
        if (costs[i].n_inf)
        {
            printf("%lu ", costs[i].value);
        }
        else
        {
            printf("I ");
        }
    }
    printf("\n");

    printf("parents:   ");
    for (int i = 1; i <= N; ++i)
    {
        printf("%d ", parents[i]);
    }
    printf("\n");

    printf("processed: ");
    for (int i = 1; i <= N; ++i)
    {
        printf("%d ", processed[i]);
    }
    printf("\n");
#endif

    dijkstra_protocol *d_protocol = malloc(sizeof(dijkstra_protocol));
    d_protocol->costs = costs;
    d_protocol->parents = parents;
    // free(costs);
    // free(parents);
    free(processed);
    return d_protocol;
}

void print_dijkstra(dijkstra_protocol *d_protocol, const int N, const int S, const int F)
{
    for (int i = 1; i <= N; ++i)
    {
        if (d_protocol->costs[i] == UINT64_MAX)
        {
            printf("oo ");
        }
        else if (d_protocol->costs[i] > INT_MAX)
        {
            printf("INT_MAX+ ");
        }
        else
        {
            printf("%lu ", d_protocol->costs[i]);
        }
    }

    printf("\n");

    if (d_protocol->costs[F] == UINT64_MAX)
    {
        printf("no path");
    }
    else if (d_protocol->costs[F] > INT_MAX)
    {
        int c = 0;
        int i = F;
        if (d_protocol->costs[i] > INT_MAX)
        {
            c++;
        }
        while (i != S)
        {
            i = d_protocol->parents[i];
            if (d_protocol->costs[i] > INT_MAX)
            {
                c++;
            }
        }

        if (c > 1)
        {
            printf("overflow");
        }
        else
        {
            int i = F;
            printf("%d ", i);
            while (i != S)
            {
                i = d_protocol->parents[i];
                printf("%d ", i);
            }
        }
    }
    else
    {
        int i = F;
        printf("%d ", i);
        while (i != S)
        {
            i = d_protocol->parents[i];
            printf("%d ", i);
        }
    }

    free(d_protocol->parents);
    free(d_protocol->costs);
    free(d_protocol);
}

int main()
{
#ifdef DEBUG
    FILE *in = freopen("../in.txt", "r", stdin);
#endif

    unsigned N, S, F, M;
    if (scanf("%u %u %u %u", &N, &S, &F, &M) < 4)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    uint64_t(*adjacency_matrix)[N + 1] = input_graph(N, S, F, M);
    if (adjacency_matrix == NULL)
    {
        free(adjacency_matrix);
        return EXIT_SUCCESS;
    }

    print_dijkstra(dijkstra_algorithm(S, adjacency_matrix, N), N, S, F);

#ifdef DEBUG
    printf("             1 2 3 4 5 6 7 8 9 0\n");
    printf("--------------------------------\n");
    for (int i = 1; i < N + 1; ++i)
    {
        printf("children %d : ", i);
        for (int j = 1; j < N + 1; ++j)
        {
            printf("%lu ", adjacency_matrix[i][j]);
        }
        printf("\n");
    }
#endif

    free(adjacency_matrix);
    return EXIT_SUCCESS;
}