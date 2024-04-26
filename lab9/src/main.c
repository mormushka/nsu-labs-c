#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "list.h"
#include "graph.h"
#include "dijkstra.h"
#include "output.h"

int main()
{
    unsigned num_vertices, start, finish, num_edges;
    if (scanf("%u %u %u %u", &num_vertices, &start, &finish, &num_edges) < 4)
    {
        fprintf(stderr, "Input error %d\n", __LINE__);
        return EXIT_FAILURE;
    }

    unsigned(*graph)[num_vertices + 1] = input_graph(num_vertices, start, finish, num_edges);
    if (graph == NULL)
    {
        free(graph);
        return EXIT_SUCCESS;
    }

    shortest_paths *s_paths = dijkstra_algorithm(start, graph, num_vertices);

    free(graph);

    print_path_array(s_paths);
    print_shortest_path(s_paths, finish);

    destroy_shortest_path(s_paths);

    return EXIT_SUCCESS;
}
