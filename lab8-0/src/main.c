#include <stdio.h>
#include <stdlib.h>

enum
{
    N_MAX = 5000,
    I_MAX = 2147483647,
};

typedef struct Item
{
    int start;
    int end;
    int weight;
} Item;

void createItem(int start, int end, int weight, Item *item)
{
    item->end = end;
    item->start = start;
    item->weight = weight;
}

int readData(FILE *flIn, int *n, int *m)
{
    if (fscanf(flIn, "%d", n) < 1)
    {
        printf("bad number of lines");
        return 1;
    }
    if (*n < 0 || *n > N_MAX)
    {
        printf("bad number of vertices");
        return 1;
    }
    if (fscanf(flIn, "%d", m) < 1)
    {
        printf("bad number of lines");
        return 1;
    }
    int maxim = ((*n + 1) / 2) * (*n);
    if (*m < 0 || *m > maxim)
    {
        printf("bad number of edges");
        return 1;
    }
    if (*n == 0)
    {
        printf("no spanning tree");
        return 1;
    }
    return 0;
}

int buildGraph(FILE *fl, int m, int n, Item *itemList)
{
    int start, end;
    long long int weight;
    for (int i = 0; i < m; ++i)
    {
        if (fscanf(fl, "%d %d %lld", &start, &end, &weight) < 1)
        {
            printf("bad number of lines");
            return 1;
        }
        if (start < 0 || start > n || end < 0 || end > n)
        {
            printf("bad vertex");
            return 1;
        }
        if (weight > I_MAX || weight < 0)
        {
            printf("bad length");
            return 1;
        }
        createItem(start, end, (int)weight, &itemList[i]);
    }
    return 0;
}

int isInTree(int trees[], int val)
{
    if (trees[val] == val)
    {
        return val;
    }
    trees[val] = isInTree(trees, trees[val]);
    return trees[val];
}

void swap(int *a, int *b)
{
    int temp = *b;
    *b = *a;
    *a = temp;
}

void uniteTrees(int startNode, int endNode, int trees[])
{
    int start = isInTree(trees, startNode);
    int end = isInTree(trees, endNode);
    swap(&start, &end);
    if (start != end)
    {
        trees[start] = end;
    }
}

int compare(const void *a, const void *b)
{
    Item *a1 = (Item *)a;
    Item *b1 = (Item *)b;
    return a1->weight - b1->weight;
}

int kruskalAlgorithm(Item *itemList, char *marker, int n, int m)
{
    int trees[n];
    int index = 0;
    int cost = 0;
    for (int i = 0; i < n; ++i)
    {
        trees[i] = i;
        marker[i] = 1;
    }

    if (m > 0)
    {
        qsort(itemList, m, sizeof(*itemList), compare);
    }

    for (int i = 0; i < m; ++i)
    {
        Item currentItem = itemList[i];
        int start = currentItem.start;
        int end = currentItem.end;
        int weight = currentItem.weight;
        if (isInTree(trees, start - 1) != isInTree(trees, end - 1))
        {
            cost += weight;
            uniteTrees(start - 1, end - 1, trees);
            itemList[index].start = start;
            itemList[index].end = end;
            marker[start - 1] = 0;
            marker[end - 1] = 0;
            index++;
        }
    }
    return index;
}

int checkTree(char *marker, int n)
{
    for (int i = 0; i < n; ++i)
        if (marker[i])
            return 1;
    return 0;
}

int main()
{
    int n, m;
    FILE *fl = fopen("in.txt", "r");
    if (readData(fl, &n, &m))
    {
        fclose(fl);
        return 0;
    }

    Item *itemList = malloc(sizeof(Item) * m);
    if (buildGraph(fl, m, n, itemList))
    {
        free(itemList);
        fclose(fl);
        return 0;
    }

    char marker[n];
    int index = kruskalAlgorithm(itemList, marker, n, m);
    int flag = checkTree(marker, n);

    if (flag && n != 1)
    {
        printf("no spanning tree\n");
    }
    else
    {
        for (int i = 0; i < index; ++i)
        {
            if (itemList[i].start > itemList[i].end)
            {
                swap(&itemList[i].start, &itemList[i].end);
            }
            printf("%d %d\n", itemList[i].start, itemList[i].end);
        }
    }
    free(itemList);
    fclose(fl);
    return 0;
}
