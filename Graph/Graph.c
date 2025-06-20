/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA2
* File: Graph.c
* Description: Has an underlying structure for BFS
***/

#include "Graph.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

// Structs -----
typedef struct GraphObj {
    List* adj;
    int* color;
    int* parent;
    int* dist;
    int source;
    int order;
    int size;
} GraphObj;


// Constructors and Destructors

// creates and returns a new empty Graph
Graph newGraph(int n) {
    Graph G = malloc(sizeof(GraphObj));
    if (!G) {
        fprintf(stderr, "Graph Error: unable to allocate memory for Graph\n");
        exit(EXIT_FAILURE);
    }

    G->adj = calloc(n + 1, sizeof(List));
    G->color = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->dist = calloc(n + 1, sizeof(int));

    if (G->adj == NULL || G->color == NULL || G->parent == NULL || G->dist == NULL) {
        fprintf(stderr, "Graph Error: unable to allocate memory for Graph properties\n");
        freeGraph(&G);  
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= n; i++) {
        G->adj[i] = newList();
        G->parent[i] = NIL;
        G->dist[i] = INF;
    }

    G->order = n;
    G->size = 0;
    G->source = NIL;

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
    free((*pG)->color);
    free((*pG)->parent);
    free((*pG)->dist);
    free(*pG);
    *pG = NULL;
}



// Access Functions

// returns size of G
int getSize(Graph G) {
    if (!G) {
        fprintf(stderr, "Graph Error: calling getSize() on a NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    return G->size;
}

// returns order of G
int getOrder(Graph G) {
    if (!G) {
        fprintf(stderr, "Graph Error: calling getOrder() on a NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    return G->order;
}

// returns source vertex of G most recently used in BFS()
int getSource(Graph G) {
    if (!G) {
        fprintf(stderr, "Graph Error: calling getSource() on a NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    return G->source;
}

// returns parent of vertex u in BFS tree
// precondition: 1 <= u <= getOrder(G)
int getParent(Graph G, int u) {
    if (!G) {
        fprintf(stderr, "Graph Error: calling getParent() on a NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "Graph Error: calling getParent() with u out of range [1, %d]\n", getOrder(G));
        exit(EXIT_FAILURE);
    }

    if (G->source == NIL) {
        return NIL;
    }

    return G->parent[u];
}

// returns distance from most recent BFS source to u. Returns INF if BFS has not been called
int getDist(Graph G, int u) {
    if (!G || (u < 1 || u > getOrder(G))) {
        if (!G) {
            fprintf(stderr, "Graph Error: NULL Graph in getDist()\n");
        } else {
            fprintf(stderr, "Graph Error: u=%d out of bounds [1, %d] in getDist()\n", u, getOrder(G));
        }
        exit(EXIT_FAILURE);
    }

    return (G->source == NIL ? INF : *(G->dist + u));
}



// appends verticies of the shortest path in G from source to u to List L. Appends NIL to L if the path does not exist
// precondition: geteSource(G) != NIL
// precondition: 1 <= u <= getOrder(G)
void getPath(List L, Graph G, int u) {
    if (!L || !G || u < 1 || u > getOrder(G)) {
        if (!L) {
            fprintf(stderr, "Graph Error: getPath() called on NULL List\n");
        } else if (!G) {
            fprintf(stderr, "Graph Error: getPath() called on NULL Graph\n");
        } else {
            fprintf(stderr, "Graph Error: u=%d out of bounds [1, %d] in getPath()\n", u, getOrder(G));
        }
        exit(EXIT_FAILURE);
    }

    if (G->source == NIL) {
        fprintf(stderr, "Graph Error: getPath() called before BFS() was executed\n");
        exit(EXIT_FAILURE);
    }

    if (u == G->source) {
        append(L, u);
        return;
    }

    if (G->parent[u] == NIL) {
        append(L, NIL);
        return;
    }

    int current = u;
    int count = 0;
    int tempPath[getOrder(G)];
    int index = 0;

    while (current != G->source && current != NIL) {
        tempPath[index++] = current;
        current = G->parent[current];
        
        if (++count > getOrder(G)) {
            fprintf(stderr, "Graph Error: detected loop in parent pointers\n");
            append(L, NIL); 
            return;
        }
    }

    if (current == G->source) {
        tempPath[index++] = G->source;
    } else {
        append(L, NIL);
        return;
    }

    for (int i = index - 1; i >= 0; i--) {
        append(L, tempPath[i]);
    }
}

// Manipulation Procedures

// deletes all edges of G, restoring it to its original state
void makeNull(Graph G) {
    if (!G) {
        fprintf(stderr, "Graph Error: makeNull() called on NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0, limit = G->order; i != limit; ) {
        if (i < limit) {
            clear(G->adj[i++]);
        } else {
            break;
        }

        if (limit < 0) {
            break;
        }
    }

    G->size = 0;
    G->source = NIL;
}

// inserts a new edge joining u to v
// precondition: 1 <= u <= getOrder(G)
void addEdge(Graph G, int u, int v) {
    addArc(G, u, v);
    addArc(G, v, u);
    G->size--;
}

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

// runs the BFS algorithm on Graph G with source s
void BFS(Graph G, int s) {
    int order = getOrder(G);
    for (int i = 1; i <= order; i++) {
        G->color[i] = NIL;
        G->dist[i] = INF;
        G->parent[i] = NIL;
    }

    G->source = s;
    G->color[s] = 1;
    G->dist[s] = 0;
    G->parent[s] = NIL;

    List queue = newList();
    append(queue, s);

    while (length(queue) != 0) {
        int u = front(queue);
        deleteFront(queue);
        List L = G->adj[u];
        
        moveFront(L);
        while (index(L) != -1) {

            int v = get(L);
            if (G->color[v] == NIL) {
                G->color[v] = 1;
                G->dist[v] = G->dist[u] + 1;
                G->parent[v] = u;
                append(queue, v);
            }
            moveNext(L);
        }
        G->color[u] = 2;
    }
    freeList(&queue);
}




// Other Functions

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
