#include "dijkstra.h"

int find_lovest_cost_node(uint64_t *costs, char *processed, const int num_vertices)
{
    int lovest_cost_node = 0;
    uint64_t lovest_cost = UINT64_MAX;

    for (int i = 1; i <= num_vertices; ++i)
    {
        if ((!processed[i]) && (costs[i] < lovest_cost))
        {
            lovest_cost = costs[i];
            lovest_cost_node = i;
        }
    }

    return lovest_cost_node;
}

void init_costs(uint64_t *costs, const unsigned num_vertices)
{
    for (unsigned i = 1; i <= num_vertices; ++i)
    {
        costs[i] = UINT64_MAX;
    }
}

shortest_paths *dijkstra_algorithm(const int start, void *v_graph, const int num_vertices)
{
    unsigned(*graph)[num_vertices + 1] = v_graph;

    uint64_t *costs = calloc(num_vertices + 1, sizeof(uint64_t));
    list **parents = calloc(num_vertices + 1, sizeof(list));
    char *processed = calloc(num_vertices + 1, sizeof(char));
    if ((costs == NULL) || (parents == NULL) || (processed == NULL))
    {
        free(costs);
        free(parents);
        free(processed);
        fprintf(stderr, "Memory allocation failed %d\n", __LINE__);
        return NULL;
    }

    init_costs(costs, num_vertices);
    costs[start] = 0;

    int node = find_lovest_cost_node(costs, processed, num_vertices);
    while (node)
    {
        uint64_t cost_node = costs[node];
        for (int i = 1; i <= num_vertices; ++i)
        {
            if (graph[node][i])
            {
                uint64_t new_cost = cost_node + graph[node][i];
                if (new_cost <= costs[i])
                {
                    costs[i] = new_cost;
                    push(&parents[i], node);
                }
            }
        }
        processed[node] = 1;
        node = find_lovest_cost_node(costs, processed, num_vertices);
    }

    shortest_paths *d_protocol = malloc(sizeof(shortest_paths));
    d_protocol->costs = costs;
    d_protocol->parents = parents;
    d_protocol->num_vertices = num_vertices;
    d_protocol->start = start;
    free(processed);
    return d_protocol;
}

void destroy_shortest_path(shortest_paths *s_paths)
{
    free(s_paths->costs);
    for (int i = 1; i <= s_paths->num_vertices; ++i)
    {
        list *tmp = s_paths->parents[i];
        destroy_list(tmp);
    }
    free(s_paths->parents);
    free(s_paths);
}

int check_num_paths(list **parents, int f)
{
    if (parents[f] == NULL)
    {
        return 1;
    }
    else
    {
        int num_path = 0;
        list *tmp = parents[f];
        while (tmp != NULL)
        {
            num_path += check_num_paths(parents, tmp->value);
            tmp = tmp->next;
        }
        return num_path;
    }
}