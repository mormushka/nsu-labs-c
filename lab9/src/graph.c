#include "graph.h"

int is_bad_vertex(const unsigned v, const unsigned num_vertices)
{
    return (v == 0) || (v > num_vertices);
}

int input_line(unsigned *s_edge, unsigned *f_edge, unsigned *len_edge, const unsigned num_vertices)
{
    int a = scanf("%u %u %u", s_edge, f_edge, len_edge);
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

    if (is_bad_vertex(*s_edge, num_vertices) || is_bad_vertex(*f_edge, num_vertices))
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

void *input_graph(const unsigned num_vertices, const unsigned start, const unsigned finish, const unsigned num_edges)
{
    if (num_vertices > 5000)
    {
        printf("bad number of vertices");
        return NULL;
    }

    if (is_bad_vertex(start, num_vertices) || is_bad_vertex(finish, num_vertices))
    {
        printf("bad vertex");
        return NULL;
    }

    if (num_edges > num_vertices * (num_vertices + 1) / 2)
    {
        printf("bad number of edges");
        return NULL;
    }

    unsigned(*graph)[num_vertices + 1] = calloc((num_vertices + 1) * (num_vertices + 1), sizeof(unsigned));

    for (unsigned i = 0; i < num_edges; ++i)
    {
        unsigned s_edge, f_edge;
        unsigned len_edge;

        if (!input_line(&s_edge, &f_edge, &len_edge, num_vertices))
        {
            free(graph);
            return NULL;
        }

        graph[s_edge][f_edge] = len_edge;
        graph[f_edge][s_edge] = len_edge;
    }

    return graph;
}