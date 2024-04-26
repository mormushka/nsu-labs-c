#ifndef OUTPUT_H
#define OUTPUT_H

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include "dijkstra.h"

void print_path_array(shortest_paths *s_paths);
void print_shortest_path(shortest_paths *s_paths, const int finish);

#endif
