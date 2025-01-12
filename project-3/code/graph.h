/**
 * @file graph.h
 * @brief header file for graph.c
 *
 */
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief adjacency list of a vertex
 * 
 */
typedef struct _vertex Vertex;
struct _vertex
{
    int dest; /* destination vertex */
    int distance; /* distance from source vertex */
    Vertex *next; /* next vertex in the list */
};

Vertex *addEdge(Vertex *src, int dest, int distance);
void freeVertex(Vertex *v);

/**
 * @brief directed graph, can have parallel edges and self-loops
 * 
 */
typedef struct _graph Graph;
struct _graph
{
    int numVertices; /* number of vertices */
    Vertex **vertices; /* array of adjacency lists */
};

Graph *createGraph(FILE *fp);
void printGraph(FILE *fp, Graph *g);
void freeGraph(Graph *g);

/**
 * @brief path from source vertex to destination vertex, stored as a dynamically allocated array
 * 
 */
typedef struct _path Path;
struct _path
{
    int *vertices; /* array of vertices */
    int num, size; /* number of vertices, size of array */
    int length; /* length of the path */
};

Path *createPath(int src);
Path *copyPath(Path *path);
Path *stepPath(Path *path, int dest, int distance);
void freePath(Path *path);
void printPath(FILE *fp, Path *path);
bool badPath(Path *path, int* temp);

/**
 * @brief priority queue of paths, stored as a binary heap
 * 
 */
typedef struct _pathPQ PathPQ;
struct _pathPQ
{
    int size, max;
    Path **heap;
};

PathPQ *createPathPQ(void);
void enqueuePathPQ(PathPQ *pq, Path *path);
Path *dequeuePathPQ(PathPQ *pq);
void freePathPQ(PathPQ *pq);
void printPathPQ(FILE *fp, PathPQ *pq);
inline bool isEmptyPathPQ(PathPQ *pq){return pq->size == 0;}