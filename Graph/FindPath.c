/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA2
* File: FindPath.c
* Description: Implment the underlying structure for BFS
***/

#include <string.h>
#include "Graph.h"

int main(int argc, char *argv[])
{
    Graph G = NULL;
    List L = NULL;
    FILE *in = NULL;
    FILE *out = NULL;

    if (argc == 3) {
        in = fopen(argv[1], "r");
        if (in) {
            out = fopen(argv[2], "w");
            if (out) {
                // Successfully opened both files
            } else {
                fprintf(stderr, "Not able to write to file\n");
                fclose(in);
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Not able to read file\n");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }

    int size = 0;
    if (fscanf(in, " %d", &size) != 1)
    {
        fprintf(stderr, "Failed to read graph size\n");
        fclose(in);
        fclose(out);
        exit(EXIT_FAILURE);
    }
    G = newGraph(size);

    int src = 1;
    int dst = 1;
    while (fscanf(in, " %d %d", &src, &dst) == 2 && (src != 0 || dst != 0))
    {
        addEdge(G, src, dst);
    }

    printGraph(out, G);

    L = newList();
    while (fscanf(in, " %d %d", &src, &dst) == 2 && (src != 0 || dst != 0))
    {
        BFS(G, src);
        getPath(L, G, dst);

        if (length(L) == 0 || getDist(G, src) < 0)
        {
            fprintf(out, "\nThe distance from %d to %d is infinity\n", src, dst);
            fprintf(out, "No %d-%d path exists\n", src, dst);
        }
        else
        {
            fprintf(out, "\nThe distance from %d to %d is %d\n", src, dst, getDist(G, dst));
            fprintf(out, "A shortest %d-%d path is: ", src, dst);
            printList(out, L);
        }
        fprintf(out, "\n");

        clear(L);
    }

    // Clean up
    freeList(&L);
    freeGraph(&G);
    fclose(in);
    fclose(out);

    return 0;
}
