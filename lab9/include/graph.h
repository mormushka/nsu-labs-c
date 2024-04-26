#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int is_bad_vertex(const unsigned v, const unsigned num_vertices);
int input_line(unsigned *s_edge, unsigned *f_edge, unsigned *len_edge, const unsigned num_vertices);
void *input_graph(const unsigned num_vertices, const unsigned start, const unsigned finish, const unsigned num_edges);

#endif
