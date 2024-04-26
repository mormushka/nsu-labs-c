#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "list.h"

typedef struct shortest_paths
{
    uint64_t *costs;
    list **parents;
    int num_vertices;
    int start;
} shortest_paths;

int find_lovest_cost_node(uint64_t *costs, char *processed, const int num_vertices);
void init_costs(uint64_t *costs, const unsigned num_vertices);
shortest_paths *dijkstra_algorithm(const int start, void *v_graph, const int num_vertices);
void destroy_shortest_path(shortest_paths *s_paths);
int check_num_paths(list **parents, int f);

#endif