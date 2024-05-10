#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

typedef struct t_memory
{
    int count;
    int index;
    void *buffer;
} t_memory;

t_memory create_memory(int count, int size);
void destroy_memory(t_memory *memory);

#endif
