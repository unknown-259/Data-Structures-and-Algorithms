/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA3
* File: Graph.c
* Description: Has an underlying structure for BFS
***/

#include "Graph.h"
#include "List.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

// Structs
typedef struct GraphObj {
    List* adj;
    int* parent;
    int* color;
    int* fin;
    int* disc;
    int order;
    int size;
} GraphObj;

void DFSVisit(Graph G, int u, int* time, List S);

// Constructors and Destructors

// creates and returns a new empty Graph
Graph newGraph(int n) {
    Graph G;
    G = malloc(sizeof(struct GraphObj));
    G->adj = malloc((n+1) * sizeof(List));
    G->parent = malloc((n+1) * sizeof(int));
    G->color = malloc((n+1) * sizeof(int));
    G->fin = malloc((n+1) * sizeof(int));
    G->disc = malloc((n+1) * sizeof(int));
    G->order = n;
    G->size = 0;
    for (int i = 1; i <= n; i++) {
        G->adj[i] = newList();
        G->parent[i] = NIL;
        G->color[i] = WHITE; 
        G->fin[i] = UNDEF;
        G->disc[i] = UNDEF;
    }
    return G;
}

// Frees all heap memory associated with *pG and sets *pG to NULL
void freeGraph(Graph* pG) {
    if (pG == NULL || *pG == NULL) {
        return; // Nothing to free
    }

    for (int i = 0; i <= (*pG)->order; i++) {
        freeList(&(*pG)->adj[i]);
    }

    free((*pG)->adj);
    free((*pG)->parent);
    free((*pG)->color);
    free((*pG)->fin);
    free((*pG)->disc);
    free(*pG);
    *pG = NULL;
}

// Access Functions

// returns order of G
int getOrder(Graph G) {
    if (!G) {
        fprintf(stderr, "Graph Error: calling getOrder() on a NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    return G->order;
}

// returns size of G
int getSize(Graph G) {
    if (!G) {
        fprintf(stderr, "Graph Error: calling getSize() on a NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    return G->size;
}

// returns parent of vertex u in BFS tree
// precondition: 1 <= u <= getOrder(G)
int getParent(Graph G, int u) {
    if (!G) {
        fprintf(stderr, "Graph Error: calling getParent() on a NULL Graph\n");
        exit(EXIT_FAILURE);
    } else if (u < 1) {
        fprintf(stderr, "Graph Error: calling getParent() with u out of range [1, %d]\n", getOrder(G));
        exit(EXIT_FAILURE);
    } else if (u > getOrder(G)) {
        fprintf(stderr, "Graph Error: calling getParent() with u out of range [1, %d]\n", getOrder(G));
        exit(EXIT_FAILURE);
    }
    return G->parent[u];
}

int getDiscover(Graph G, int u){
    if (u < 1){
        printf("Graph Error: calling getDiscover() on an invalid vertex index\n");
        exit(EXIT_FAILURE);
    } else if (u > getOrder(G)) {
        printf("Graph Error: calling getDiscover() on an invalid vertex index\n");
        exit(EXIT_FAILURE);
    }
    return G->disc[u];
}

int getFinish(Graph G, int u){
    if (u < 1){
        fprintf(stderr, "Graph Error: calling getFinish(), vertex is out of range\n");
        exit(EXIT_FAILURE);
    } else if (u > getOrder(G)) {
        fprintf(stderr, "Graph Error: calling getFinish(), vertex is out of range\n");
        exit(EXIT_FAILURE);
    }
    
    return G->fin[u];
}

// Manipulation Procedures

// inserts a new directed edge from u to v
// precondition: 1 <= u <= getOrder(G)
void addArc(Graph G, int u, int v) {
    if (!G) {
        fprintf(stderr, "Graph Error: addArc() called on NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    int order = getOrder(G);
    if ((u < 1 || u > order) || (v < 1 || v > order)) {
        if (u < 1 || u > order) {
            fprintf(stderr, "Graph Error: addArc() called with u=%d out of range [1, %d]\n", u, order);
        }
        if (v < 1 || v > order) {
            fprintf(stderr, "Graph Error: addArc() called with v=%d out of range [1, %d]\n", v, order);
        }
        exit(EXIT_FAILURE);
    }

    List adjList = G->adj[u];
    if (adjList != NULL) {
        append(adjList, v);
        G->size++;
    } else {
        fprintf(stderr, "Graph Error: adjacency list for vertex %d is NULL\n", u);
        exit(EXIT_FAILURE);
    }
}

// inserts a new edge joining u to v
// precondition: 1 <= u <= getOrder(G)
void addEdge(Graph G, int u, int v) {
    addArc(G, u, v);
    addArc(G, v, u);
    G->size--;
}

void DFS(Graph G, List S) {
    // Ensure proper size of the input list S for the graph order
    if (length(S) != getOrder(G)) {
        fprintf(stderr, "Error: List size and graph order do not match.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize all vertices
    for (int i = 1; i <= getOrder(G); i++) {
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->disc[i] = UNDEF;
        G->fin[i] = UNDEF;
    }

    int time = 0;
    List temp = copyList(S);  // Temporary copy of S for traversal
    clear(S);                  // Clear S to store DFS finish order

    // Traverse each vertex in temp list
    moveFront(temp);
    while (index(temp) >= 0) {
        int u = get(temp);
        if (G->color[u] == WHITE) {
            DFSVisit(G, u, &time, S);
        }
        moveNext(temp);
    }
    freeList(&temp);
}

void DFSVisit(Graph G, int u, int* time, List S) {
    G->color[u] = GRAY;
    G->disc[u] = ++(*time);

    // Explore adjacency list of u
    moveFront(G->adj[u]);
    while (index(G->adj[u]) >= 0) {
        int v = get(G->adj[u]);
        if (G->color[v] == WHITE) {
            G->parent[v] = u;
            DFSVisit(G, v, time, S);
        }
        moveNext(G->adj[u]);
    }
    
    G->color[u] = BLACK;
    G->fin[u] = ++(*time);
    prepend(S, u);  // Insert vertex at the beginning of S
}


// Other Functions

Graph transpose(Graph G) {
    int numVertices = getOrder(G);
    Graph T = newGraph(numVertices);

    for (int u = 1; u <= numVertices; ++u) {
        for (moveFront(G->adj[u]); index(G->adj[u]) >= 0; moveNext(G->adj[u])) {
            addArc(T, get(G->adj[u]), u); // Reverse each edge
        }
    }
    return T;
}

Graph copyGraph(Graph G) {
    int numVertices = getOrder(G);
    Graph C = newGraph(numVertices);

    for (int u = 1; u <= numVertices; ++u) {
        List adjList = G->adj[u];
        for (moveFront(adjList); index(adjList) >= 0; moveNext(adjList)) {
            int v = get(adjList);
            addArc(C, u, v);
        }
    }
    return C;
}

// prints the adjacency list representation of G to file pointed to by out
void printGraph(FILE* out, Graph G) {
    if (!G) {
        fprintf(stderr, "Graph Error: printGraph() invoked on NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= G->order; i++) {
        fprintf(out, "%d:", i);
        List adjList = G->adj[i];

        for (moveFront(adjList); index(adjList) != NIL; moveNext(adjList)) {
            fprintf(out, " %d", get(adjList));
        }

        fprintf(out, "\n");
    }
}