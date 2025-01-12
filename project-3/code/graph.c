/**
 * @file graph.c
 * @brief Implementation of graph.h
 * 
 */
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Adds an edge to the given vertex.
 *
 * This function adds an edge to the given vertex.
 *
 * @param src adjacency list of the source vertex
 * @param dest destination of the edge
 * @param distance length of the edge
 * @return Vertex* adjacency list of the source vertex
 */
Vertex *addEdge(Vertex *src, int dest, int distance)
{
    /* This is the first edge. */
    if (src == NULL)
    {
        src = (Vertex *)malloc(sizeof(Vertex));
        assert(src != NULL);
        src->dest = dest;
        src->distance = distance;
        src->next = NULL;
        return src;
    }

    /* Traverse to the last edge. */
    Vertex *temp = src;
    while (temp->next != NULL)
        temp = temp->next;
    /* Add the edge. */
    temp->next = (Vertex *)malloc(sizeof(Vertex));
    assert(temp->next);
    temp->next->dest = dest;
    temp->next->distance = distance;
    temp->next->next = NULL;
    return src;
}

/**
 * @brief Frees the given vertex and all its edges.
 *
 * This function frees the given vertex and all its edges.
 *
 * @param v vertex to be freed
 */
void freeVertex(Vertex *v)
{
    if (v == NULL)
        return;
    freeVertex(v->next);
    free(v);
}

/**
 * @brief Creates a graph from the given file pointer.
 *
 * This function creates a graph from the given file pointer.
 *
 * The file format is as follows:
 * numVertices numEdges
 * src1 dest1 distance1
 * src2 dest2 distance2
 * ...
 *
 * @param fp file pointer
 * @return Graph* graph
 */
Graph *createGraph(FILE *fp)
{
    /* Read the number of vertices and edges. */
    int numVertices, numEdges;
    if (fscanf(fp, "%d %d", &numVertices, &numEdges) != 2)
    {
        fprintf(stderr, "[createGraph] Error: Invalid file format.\n");
        exit(EXIT_FAILURE);
    }

    /* Create the graph. */
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    assert(graph != NULL);
    graph->numVertices = numVertices;

    /* Allocate memory for the vertices.
     * We use numVertices + 1 because we index the vertices from 1.
     */
    graph->vertices = (Vertex **)malloc(sizeof(Vertex *) * (numVertices + 1));
    assert(graph->vertices != NULL);

    /* Initialize the vertices, with no edges. */
    for (int i = 0; i < numVertices; i++)
        graph->vertices[i] = NULL;

    /* Add the edges. */
    for (int i = 1; i <= numEdges; i++)
    {
        int src, dest, distance;
        if (fscanf(fp, "%d %d %d", &src, &dest, &distance) != 3)
        {
            fprintf(stderr, "[createGraph] Error: Invalid file format.\n");
            exit(EXIT_FAILURE);
        }
        graph->vertices[src] = addEdge(graph->vertices[src], dest, distance);
    }
    return graph;
}

/**
 * @brief Prints the graph to the given file pointer.
 *
 * This function is used for debugging purposes.
 *
 * It prints the graph in the following format:
 * Vertex 0: 1(2) 2(3)
 * Vertex 1: 0(2) 2(4)
 * Vertex 2: 0(3) 1(4)
 *
 * @param fp file pointer
 * @param graph graph to be printed
 */
void printGraph(FILE *fp, Graph *graph)
{
    for (int i = 1; i <= graph->numVertices; i++)
    {
        fprintf(fp, "Vertex %d: ", i);
        Vertex *temp = graph->vertices[i];
        if (temp == NULL)
        {
            fprintf(fp, "No edges.\n");
            continue;
        }
        while (temp != NULL)
        {
            fprintf(fp, "%d(%d) ", temp->dest, temp->distance);
            temp = temp->next;
        }
        fprintf(fp, "\n");
    }
}

/**
 * @brief Frees the given graph.
 *
 * This function frees the given graph.
 *
 * @param g graph to be freed
 */
void freeGraph(Graph *g)
{
    /* Free all the vertices. */
    for (int i = 0; i < g->numVertices; i++)
        freeVertex(g->vertices[i]);
    /* Free the array of vertices. */
    free(g->vertices);
    free(g);
}

/**
 * @brief Create a Path object
 *
 * This function creates a path object with the given source vertex.
 *
 * Default size is 64.
 * @param src source vertex
 * @return Path* path
 */
Path *createPath(int src)
{
    Path *path = (Path *)malloc(sizeof(Path));
    assert(path != NULL);
    path->size = 64;
    path->num = 0;
    path->length = 0;
    path->vertices = (int *)malloc(sizeof(int) * path->size);
    assert(path->vertices != NULL);
    path->vertices[0] = src;
    return path;
}

/**
 * @brief Copies the given path.
 *
 * This function copies the given path.
 *
 * @param path path to be copied
 * @return Path* copied path
 */
Path *copyPath(Path *path)
{
    Path *newPath = (Path *)malloc(sizeof(Path));
    assert(newPath != NULL);
    newPath->size = path->size;
    newPath->num = path->num;
    newPath->length = path->length;
    newPath->vertices = (int *)malloc(sizeof(int) * newPath->size);
    assert(newPath->vertices != NULL);
    memcpy(newPath->vertices, path->vertices, sizeof(int) * newPath->size);
    return newPath;
}

/**
 * @brief Adds a step to the given path.
 *
 * This function adds a step to the given path.
 *
 * @param path path to be copied
 * @param dest destination vertex
 * @param distance distance to the destination vertex
 * @return Path* new path
 */
Path *stepPath(Path *path, int dest, int distance)
{
    if (path->num + 1 == path->size)
    {
        path->size *= 2;
        path->vertices = (int *)realloc(path->vertices, sizeof(int) * path->size);
        assert(path->vertices != NULL);
    }
    Path *newPath = copyPath(path);
    newPath->vertices[++newPath->num] = dest;
    newPath->length += distance;
    return newPath;
}

/**
 * @brief Compares two paths.
 *
 * This function compares two paths.
 *
 * @param p1 path 1
 * @param p2 path 2
 * @return int -1 if p1 < p2, 0 if p1 == p2, 1 if p1 > p2
 */
inline int comparePath(Path *p1, Path *p2)
{
    if (p1->length < p2->length)
        return -1;
    if (p1->length > p2->length)
        return 1;
    return 0;
}

/**
 * @brief Frees the given path.
 *
 * This function frees the given path.
 *
 * @param path path to be freed
 */
void freePath(Path *path)
{
    free(path->vertices);
    free(path);
}

/**
 * @brief Prints the path to the given file pointer.
 *
 * This function is used for debugging purposes.
 *
 * It prints the path in the following format:
 * length src v2 v3 ... dest
 *
 * no space at the end of the line
 * @param fp file pointer
 * @param path path to be printed
 */
void printPath(FILE *fp, Path *path)
{
    /* Print the length and the vertices. */
    fprintf(fp, "%d ", path->length);
    for (int i = 0; i < path->num; i++)
        fprintf(fp, "%d ", path->vertices[i]);
    fprintf(fp, "%d", path->vertices[path->num]);
    fprintf(fp, "\n");
}

/**
 * @brief Checks if the path is bad (repeat a loop more than twice)
 *
 * This function checks if the path is bad (repeat a loop more than twice)
 *
 * @param path path to be checked
 * @param temp an array to store the number of times each vertex is visited. It should have size equal to number of vertices, and must be set to zero before use.
 */
bool badPath(Path *path, int *temp)
{
    for (int i = 0; i <= path->num; i++)
    {
        temp[path->vertices[i]]++;
        if (temp[path->vertices[i]] > 2)
            return true;
    }
    return false;
}

/**
 * @brief Create a Path Priority Queue object
 *
 * This function creates a path priority queue.
 *
 * It uses a min-heap to store the paths.
 *
 * Default size is 63.
 *
 * @return PathPQ* path priority queue
 */
PathPQ *createPathPQ(void)
{
    /* Allocate memory for the priority queue. */
    PathPQ *pq = (PathPQ *)malloc(sizeof(PathPQ));
    assert(pq != NULL);
    pq->size = 0;
    pq->max = 64;
    pq->heap = (Path **)malloc(sizeof(Path *) * pq->max);
    assert(pq->heap != NULL);
    return pq;
}

/**
 * @brief Adds a path to the priority queue.
 *
 * This function adds a path to the priority queue.
 *
 * @param pq priority queue
 * @param path path to be added
 */
void enqueuePathPQ(PathPQ *pq, Path *path)
{
    /* Resize the heap if necessary. */
    if (pq->size + 1 == pq->max)
    {
        pq->max *= 2;
        pq->heap = (Path **)realloc(pq->heap, sizeof(Path *) * pq->max);
        assert(pq->heap != NULL);
    }

    /* Insert at the end. */
    pq->heap[++pq->size] = path;

    /* Bubble up. */
    int i = pq->size;
    while (i > 1 && comparePath(pq->heap[i], pq->heap[i / 2]) < 0)
    {
        Path *temp = pq->heap[i];
        pq->heap[i] = pq->heap[i / 2];
        pq->heap[i / 2] = temp;
        i /= 2;
    }
}

/**
 * @brief Removes the path with the minimum length from the priority queue.
 *
 * This function removes the path with the minimum length from the priority queue.
 *
 * @param pq priority queue
 * @return Path* path with the minimum length
 */
Path *dequeuePathPQ(PathPQ *pq)
{
    /* Empty queue. */
    if (pq->size == 0)
        return NULL;

    /* Only one element. */
    if (pq->size == 1)
        return pq->heap[pq->size--];

    /* Remove the first element. */
    Path *path = pq->heap[1];
    pq->heap[1] = pq->heap[pq->size--];

    /* Bubble down. */
    int i = 1;
    while (i * 2 <= pq->size)
    {
        int child = i * 2;
        if (child + 1 <= pq->size && comparePath(pq->heap[child + 1], pq->heap[child]) < 0)
            child++;
        if (comparePath(pq->heap[i], pq->heap[child]) < 0)
            break;
        Path *temp = pq->heap[i];
        pq->heap[i] = pq->heap[child];
        pq->heap[child] = temp;
        i = child;
    }
    return path;
}

/**
 * @brief Frees the given priority queue.
 *
 * This function frees the given priority queue.
 *
 * @param pq priority queue to be freed
 */
void freePathPQ(PathPQ *pq)
{
    for (int i = 1; i <= pq->size; i++)
        freePath(pq->heap[i]);
    free(pq->heap);
    free(pq);
}

/**
 * @brief Prints the priority queue to the given file pointer.
 *
 * This function is used for debugging purposes.
 *
 * It prints the priority queue in the following format:
 * length src v2 v3 ... dest
 *
 * @param fp file pointer
 * @param pq priority queue to be printed
 */
void printPathPQ(FILE *fp, PathPQ *pq)
{
    for (int i = 1; i <= pq->size; i++)
        printPath(fp, pq->heap[i]);
}
