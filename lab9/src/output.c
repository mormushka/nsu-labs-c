#include "output.h"

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
