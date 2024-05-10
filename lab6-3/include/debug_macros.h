#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#if 1
#define DEBUG
#endif

#ifdef DEBUG
#include <stdio.h>
#define DEBUG_PRINT(msg)                                                           \
    do                                                                             \
    {                                                                              \
        fprintf(stderr, "%s FILE ( %s ), LINE ( %d )\n", msg, __FILE__, __LINE__); \
    } while (0)
#else
#define DEBUG_PRINT(msg)
#endif

#endif
