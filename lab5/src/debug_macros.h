#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#if 1
#define NDEBUG
#endif

#ifndef NDEBUG
#include <stdio.h>
#define DEBUG_PRINT(msg)                                                           \
    do                                                                             \
    {                                                                              \
        fprintf(stderr, "%s FILE ( %s ), LINE ( %d )\n", msg, __FILE__, __LINE__); \
    } while (0)

#define DEBUG_FREQUENCY_TABLE(frequencies, len)                    \
    do                                                             \
    {                                                              \
        fprintf(stderr, "# FREQUENCY TABLE:\n");                   \
        for (int i = 0; i < len; i++)                              \
        {                                                          \
            if (frequencies[i])                                    \
            {                                                      \
                fprintf(stderr, "%.2x - %d\n", i, frequencies[i]); \
            }                                                      \
        }                                                          \
        fprintf(stderr, "# END FREQUENCY TABLE\n\n");              \
    } while (0)

#define DEBUG_PRINT_CODES_TABLE(codes, len)               \
    do                                                    \
    {                                                     \
        fprintf(stderr, "# CODES TABLE:\n");              \
        for (int i = 0; i < len; i++)                     \
        {                                                 \
            unsigned long long k = codes[i].code;         \
            if (codes[i].length)                          \
            {                                             \
                fprintf(stderr, "%.2x - ", i);            \
                for (int j = 0; j < codes[i].length; j++) \
                {                                         \
                    fprintf(stderr, "%llu", k & 1);       \
                    k = (k >> 1);                         \
                }                                         \
                fprintf(stderr, "\n");                    \
            }                                             \
        }                                                 \
        fprintf(stderr, "# END CODES TABLE\n\n");         \
    } while (0)

#else
#define DEBUG_PRINT(msg)
#define DEBUG_FREQUENCY_TABLE(frequencies, len)
#define DEBUG_PRINT_CODES_TABLE(codes, len)
#endif

#endif
