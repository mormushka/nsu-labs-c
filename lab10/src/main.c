#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

enum
{
    N_MAX = 100000
};

typedef struct point
{
    int64_t x, y;
} point;

point p0;

long double rotate(point a, point b, point c)
{
    int64_t v1_x = b.x - a.x;
    int64_t v1_y = b.y - a.y;
    int64_t v2_x = c.x - a.x;
    int64_t v2_y = c.y - a.y;
    return (long double)v1_x * (long double)v2_y - (long double)v1_y * (long double)v2_x;
}

void swap(point *a, point *b)
{
    point tmp = *b;
    *b = *a;
    *a = tmp;
}

uint64_t vec_len(point a, point b)
{
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

int compare(const void *vp1, const void *vp2)
{
    point *p1 = (point *)vp1;
    point *p2 = (point *)vp2;
    long double s = rotate(p0, *p1, *p2);
    if (s < 0)
    {
        return 1;
    }
    else if (s > 0)
    {
        return -1;
    }
    else
    {
        if (vec_len(p0, *p1) > vec_len(p0, *p2))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
}

int print_graham(point *points, int n)
{
    point *ch = malloc(sizeof(point) * n);
    if (ch == NULL)
    {
        return ENOMEM;
    }

    for (int i = 0; i < n; ++i)
    {
        if ((points[i].y < points[0].y) || ((points[0].y == points[i].y) && (points[i].x < points[0].x)))
        {
            swap(&points[0], &points[i]);
        }
    }
    p0 = points[0];

    qsort(&points[1], n - 1, sizeof(point), compare);

    int len_cv = 0;
    ch[len_cv++] = points[0];
    ch[len_cv++] = points[1];

    for (int i = 1; i < n; ++i)
    {
        while ((len_cv > 1) && (rotate(ch[len_cv - 2], ch[len_cv - 1], points[i]) <= 0))
        {
            len_cv--;
        }
        ch[len_cv++] = points[i];
    }

    for (int i = 0; i < len_cv; ++i)
    {
        printf("%ld %ld\n", ch[i].x, ch[i].y);
    }

    free(ch);
    return EXIT_SUCCESS;
}

int main()
{
    int n;
    if (scanf("%d", &n) < 1)
    {
        return EIO;
    }
    if ((n < 0) || (n > N_MAX))
    {
        printf("bad number of points");
        return EXIT_SUCCESS;
    }

    point points[n];
    int x, y;
    for (int i = 0; i < n; ++i)
    {
        if (scanf("%d %d", &x, &y) < 2)
        {
            printf("bad number of lines");
            return EXIT_SUCCESS;
        }
        points[i].x = x;
        points[i].y = y;
    }

    if (n == 1)
    {
        printf("%ld %ld", points[0].x, points[0].y);
        return EXIT_SUCCESS;
    }
    else if (n != 0)
    {
        return print_graham(points, n);
    }
}
