#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>

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

        FILE *fl = fopen(argv[2], "rb");
        if (fl == NULL)
        {
            printf("Error: Could not open input file %s for reading.\n", argv[2]);
            print_usage();
            return EXIT_FAILURE;
        }

        FILE *fl_out = fopen(argv[3], "wb");
        if (fl_out == NULL)
        {
            printf("Error: Could not open output file %s for writing.\n", argv[3]);
            fclose(fl);
            print_usage();
            return EXIT_FAILURE;
        }

        if (mode == 'c')
        {
            encode(fl, fl_out, 1);
        }
        else if (mode == 'd')
        {
            decode(fl, fl_out, 1);
        }

        fclose(fl);
        fclose(fl_out);
    }
    else if (argc == 1)
    {
        char mode;
        FILE *fl = fopen("in.txt", "rb");
        FILE *flOut = fopen("out.txt", "wb");
        if (fread(&mode, 1, 1, fl) < 1)
        {
            fclose(fl);
            fclose(flOut);
            return EXIT_FAILURE;
        }
        if (mode == 'c')
        {
            encode(fl, flOut, 0);
        }
        else
        {
            decode(fl, flOut, 0);
        }
        fclose(fl);
        fclose(flOut);
        return EXIT_SUCCESS;
    }
    else
    {
        print_usage();
        return EXIT_FAILURE;
    }
}
