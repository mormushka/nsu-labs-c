#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#define FREOPEN 0

#define INCORRECT_INPUT 1

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
