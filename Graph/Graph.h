/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA2
* File: Graph.h
* Description: Has an underlying structure for BFS
***/

#include <stdlib.h>
#include "List.h"

#define NIL -1
#define INF -2

// Structs
typedef struct GraphObj* Graph;

/*** Constructors-Destructors ***/
Graph newGraph(int n);
void freeGraph(Graph* pG);

/*** Access functions ***/
int getSize(Graph G);
int getOrder(Graph G);
int getSource(Graph G);
int getParent(Graph G, int u);
int getDist(Graph G, int u);
void getPath(List L, Graph G, int u);

/*** Manipulation procedures ***/
void makeNull(Graph G);
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void BFS(Graph G, int s);

/*** Other operations ***/
void printGraph(FILE* out, Graph G);