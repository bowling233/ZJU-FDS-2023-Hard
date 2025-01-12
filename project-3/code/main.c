#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    /* if no input file, read from stdin */
    FILE *input;
    if (argc == 1)
    {
        input = stdin;
        fprintf(stderr, "You're reading from standard input. Please Enter graph:\n");
    }
    else
        input = fopen(argv[1], "r");
    if (input == NULL)
    {
        fprintf(stderr, "[main] Error: Cannot open file.\n");
        exit(EXIT_FAILURE);
    }

    /* create graph */
    Graph *g = createGraph(input);
    fclose(input);

    /* set a flag to check if there is a second shortest path */
    int shortestLength = -1;
    bool secondShortest = false;

    /* create temp array for badPath */
    int *temp = (int *)calloc(g->numVertices + 1, sizeof(int));

    /* create priority queue to store paths */
    PathPQ *pq = createPathPQ();
    enqueuePathPQ(pq, createPath(1));

    /* BFS */
    while (!isEmptyPathPQ(pq))
    {
        /* explore the current shortest path */
        Path *path = dequeuePathPQ(pq);

        /* check if the path is bad
         * bad path cannot be the 2nd shortest path
         */
        memset(temp, 0, (g->numVertices + 1) * sizeof(int));
        if (badPath(path, temp))
        {
            freePath(path);
            continue;
        }

        /* complete path */
        if (path->vertices[path->num] == g->numVertices)
        {
            /* shortest path*/
            if (shortestLength == -1)
            {
                shortestLength = path->length;
                freePath(path);
                continue;
            }
            /* 2nd shortest path */
            if (path->length > shortestLength)
            {
                secondShortest = true;
                printPath(stdout, path);
                freePath(path);
                break;
            }
        }

        /* explore to the next vertex */
        int end = path->vertices[path->num];
        Vertex *temp = g->vertices[end];
        while (temp != NULL)
        {
            int dest = temp->dest;
            int distance = temp->distance;
            enqueuePathPQ(pq, stepPath(path, dest, distance));
            temp = temp->next;
        }
        freePath(path);
    }

    /* no second shortest path */
    if (!secondShortest)
        fprintf(stderr, "[Error] No second shortest path.\n");

    /* free memory */
    freePathPQ(pq);
    freeGraph(g);
    free(temp);
    fclose(stderr);
    return 0;
}