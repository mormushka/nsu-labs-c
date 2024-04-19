#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long long uint64;

// dijkstra_algorithm(int S, void *adjacency_matrix);

int is_bad_vertex(const unsigned v, const unsigned N) 
{
    return (v == 0) || (v > N);
}

int input_line(unsigned *s_edge, unsigned *f_edge, uint64 *len_edge, const unsigned N)
{
    int a = scanf("%u %u %llu", s_edge, f_edge, len_edge);
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
        return 0;
    }

    if (M > N * (N + 1) / 2)
    {
        printf("bad number of edges");
        return NULL;
    }

    uint64(*adjacency_matrix)[N] = calloc(N * N, sizeof(uint64));

    for (int i = 0; i <= M; ++i)
    {
        unsigned s_edge, f_edge;
        uint64 len_edge;

        if (!input_line(&s_edge, &f_edge, &len_edge, N))
        {
            free(adjacency_matrix);
            return NULL;
        }

        adjacency_matrix[s_edge][f_edge] = len_edge;
    }

    return adjacency_matrix;
}

int main()
{
    //FILE *in = freopen("../in.txt", "r", stdin);

    unsigned N, S, F, M;
    if (scanf("%u %u %u %u", &N, &S, &F, &M) < 4)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    uint64(*adjacency_matrix)[N] = input_graph(N, S, F, M);
    if (adjacency_matrix == NULL)
    {
        return EXIT_SUCCESS;
    }

    // uint64 *path = dijkstra_algorithm(S, adjacency_matrix);

    return EXIT_SUCCESS;
}
