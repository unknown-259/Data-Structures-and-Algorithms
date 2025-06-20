/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA3
* File: FindComponents.c
* Description: Client for Graph ADT
***/

#include "Graph.h"

int main(int argc, char* argv[]) {
    // Check number of arguments
    if (argc != 3) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        exit(EXIT_FAILURE);
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Unable to read from file.\n");
        exit(EXIT_FAILURE);
    }

    FILE *output = fopen(argv[2], "w");
    if (!output) {
        fprintf(stderr, "Unable to write to file.\n");
        fclose(input);
        exit(EXIT_FAILURE);
    }

    // Get size, make graph
    int size;
    if (fscanf(input, " %d", &size) != 1) {
        fclose(input);
        fclose(output);
        fprintf(stderr, "Invalid graph size.\n");
        exit(EXIT_FAILURE);
    }
    Graph G = newGraph(size);

    // Insert arcs into the graph
    for (int u, v; fscanf(input, " %d %d", &u, &v) == 2 && (u != 0 && v != 0); ) {
        addArc(G, u, v);
    }

    // Output adjacency list
    fprintf(output, "Adjacency list representation of G:\n");
    printGraph(output, G);

    // Initialize and prepare for DFS
    List S = newList();
    Graph T = transpose(G);
    for (int i = 1; i <= getOrder(G); ++i) {
        append(S, i);
    }
    DFS(G, S);
    DFS(T, S);

    // Identify strongly connected components
    List L = newList();
    int components = 0;
    for (moveFront(S); index(S) != -1; moveNext(S)) {
        if (getParent(T, get(S)) == NIL) {
            ++components;
        }
    }
    fprintf(output, "G contains %d strongly connected components:\n", components);

    // Extract and print each component
    int componentCounter = 0;
    for (moveBack(S); index(S) != -1; movePrev(S)) {
        if (getParent(T, get(S)) == NIL) {
            ++componentCounter;
            prepend(L, get(S));
            fprintf(output, "Component %d: ", componentCounter);
            printList(output, L);
            fprintf(output, "\n");
            clear(L);
        } else {
            prepend(L, get(S));
        }
    }

    // Clean up and free resources
    freeList(&S);
    freeGraph(&G);
    freeList(&L);
    freeGraph(&T);

    // Close files
    fclose(input);
    fclose(output);
}
