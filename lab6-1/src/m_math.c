#include "m_math.h"

int pow_m(int num, int d)
{
    return d == 0 ? 1 : num * pow_m(num, d - 1);
}

/* The maximum return degree is 30*/
int log_2(int n)
{
    /* 2^31 = 2 147 483 648 > INT_MAX 2 147 483 647*/
    for (int i = 0; i < 31; i++)
    {
        if (n < pow_m(2, i))
        {
            return i;
        }
    }
    return -1;
}
