#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

typedef struct list
{
    int value;
    struct list *next;
} list;

void push(list **l, int value);
void destroy_list(list *l);

typedef struct shortest_paths
{
    uint64_t *costs;
    list **parents;
    int num_vertices;
    int start;
} shortest_paths;

int is_bad_vertex(const unsigned v, const unsigned num_vertices);
int input_line(unsigned *s_edge, unsigned *f_edge, unsigned *len_edge, const unsigned num_vertices);
void *input_graph(const unsigned num_vertices, const unsigned start, const unsigned finish, const unsigned num_edges);

int find_lovest_cost_node(uint64_t *costs, char *processed, const int num_vertices);
void init_costs(uint64_t *costs, const unsigned num_vertices);
shortest_paths *dijkstra_algorithm(const int start, void *v_graph, const int num_vertices);
void destroy_shortest_path(shortest_paths *s_paths);

int check_num_paths(list **parents, int f);
void print_path_array(shortest_paths *s_paths);
void print_shortest_path(shortest_paths *s_paths, const int finish);

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

/*-----------------------------------------List-----------------------------------------*/

void push(list **l, int value)
{
    list *new = malloc(sizeof(list));
    new->value = value;
    new->next = *l;
    *l = new;
}

void destroy_list(list *l)
{
    while (l != NULL)
    {
        list *next = l->next;
        free(l);
        l = next;
    }
}

/*-----------------------------------------Input-graph-----------------------------------------*/

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

/*-----------------------------------------Dijksrta-algoritm-----------------------------------------*/

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

/*-----------------------------------------Output-----------------------------------------*/
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

void print_path_array(shortest_paths *s_paths)
{
    for (int i = 1; i <= s_paths->num_vertices; ++i)
    {
        if (s_paths->costs[i] == UINT64_MAX)
        {
            printf("oo ");
        }
        else if (s_paths->costs[i] > INT_MAX)
        {
            printf("INT_MAX+ ");
        }
        else
        {
            printf("%lu ", s_paths->costs[i]);
        }
    }
    printf("\n");
}

void print_shortest_path(shortest_paths *s_paths, const int finish)
{
    if (s_paths->costs[finish] == UINT64_MAX)
    {
        printf("no path\n");
        return;
    }
    else if (s_paths->costs[finish] > INT_MAX)
    {
        int num_paths = check_num_paths(s_paths->parents, finish);

        if (num_paths > 1)
        {
            printf("overflow\n");
            return;
        }
    }

    int i = finish;
    printf("%d ", i);
    while (i != s_paths->start)
    {
        i = s_paths->parents[i]->value;
        printf("%d ", i);
    }
    printf("\n");
}