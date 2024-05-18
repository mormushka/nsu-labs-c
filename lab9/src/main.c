#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include "list.h"
#include "graph.h"
#include "dijkstra.h"
#include "output.h"

int main()
{
    unsigned num_vertices, start, finish, num_edges;
    if (scanf("%u %u %u %u", &num_vertices, &start, &finish, &num_edges) < 4)
    {
        return EIO;
    }

    unsigned(*graph)[num_vertices + 1] = calloc((num_vertices + 1) * (num_vertices + 1), sizeof(unsigned));
    if (graph == NULL) 
    {
        return ENOMEM;
    }

    if (input_graph(num_vertices, graph, start, finish, num_edges))
    {
        free(graph);
        return EXIT_SUCCESS;
    }

    shortest_paths *s_paths = NULL;
    int rv = dijkstra_algorithm(start, num_vertices, graph, &s_paths);
    if (rv)
    {
        free(graph);
        return rv;
    }

    free(graph);

    print_path_array(s_paths);
    print_shortest_path(s_paths, finish);

    destroy_shortest_path(s_paths);

    return EXIT_SUCCESS;
}
