#include <stdio.h>
#include <stdlib.h>

// Path Compression
int find(int *unions, int x)
{
    if (unions[x] < 0)
        return x;
    else
        return unions[x] = find(unions, unions[x]);
}

void Union(int *unions, int x, int y)
{
    int root1 = find(unions, x);
    int root2 = find(unions, y);
    if (root1 == root2)
        return;
    if (unions[root1] < unions[root2])
    {
        unions[root1] += unions[root2];
        unions[root2] = root1;
    }
    else
    {
        unions[root2] += unions[root1];
        unions[root1] = root2;
    }
}

int main(void)
{
    int N;
    scanf("%d", &N);
    int *unions = (int *)calloc(N, sizeof(int));
    for (int i = 0; i < N; i++)
        unions[i] = -1;
    char cmd;
    int a, b;
    while ((cmd = getchar()) != 'S')
    {
        switch (cmd)
        {
        case 'I': // union
            scanf("%d%d", &a, &b);
            Union(unions, a, b);
            break;
        case 'C': // check
            scanf("%d%d", &a, &b);
            if (find(unions, a) == find(unions, b))
                printf("yes\n");
            else
                printf("no\n");
            break;
        default:
            break;
        }
    }
    // Output total number of disjoint sets
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (unions[i] < 0)
            count++;
    }
    if (count == 1)
        printf("The network is connected.\n");
    else
        printf("There are %d components.\n", count);
    return 0;
}