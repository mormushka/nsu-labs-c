#include "huffman.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc == 4)
    {
        char mode = argv[1][0];
        if (mode != 'c' && mode != 'd')
        {
            print_usage();
            return EXIT_FAILURE;
        }

        FILE *in = fopen(argv[2], "rb");
        if (in == NULL)
        {
            printf("Error: Could not open input file %s for reading. %s\n", argv[2], strerror(errno));
            print_usage();
            return errno;
        }

        FILE *out = fopen(argv[3], "wb");
        if (out == NULL)
        {
            printf("Error: Could not open output file %s for writing. %s\n", argv[3], strerror(errno));
            fclose(in);
            print_usage();
            return errno;
        }

        int rv = 0;
        if (mode == 'c')
        {
            rv = encode(in, out, 1);
        }
        else if (mode == 'd')
        {
            rv = decode(in, out, 1);
        }

        fclose(in);
        fclose(out);

        return rv;
    }
    else if (argc == 1)
    {
        FILE *in = fopen("in.txt", "rb");
        if (in == NULL)
        {
            return errno;
        }
        FILE *inOut = fopen("out.txt", "wb");
        if (in == NULL)
        {
            fclose(in);
            return errno;
        }

        char mode;
        if (fread(&mode, 1, 1, in) != 1)
        {
            fclose(in);
            fclose(inOut);
            return EIO;
        }

        int rv = 0;
        if (mode == 'c')
        {
            rv = encode(in, inOut, 0);
        }
        else
        {
            rv = decode(in, inOut, 0);
        }

        fclose(in);
        fclose(inOut);

        return rv;
    }
    else
    {
        print_usage();
        return EXIT_FAILURE;
    }
}
