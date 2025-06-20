/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA1
* File: Lex.c
* Description: Has an underlying structure as a doubly linked list with bi-directional queue 
               that includes a "cursor" to be used for iteration
***/

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

#define MAX_LEN 260

int main(int argc, char* argv[]) {
    char str[MAX_LEN];
    List strList = newList();

    // check for the correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Incorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }

    // open the file for reading
    FILE* in = fopen(argv[1], "r");
    if (in == NULL) {
        fprintf(stderr, "Unable to read from file, %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // open the file for writing
    FILE* out = fopen(argv[2], "w");
    if (out == NULL) {
        fprintf(stderr, "Unable to write to file, %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    
    // count lines in file
    int lines = 0;
    while (fgets(str, MAX_LEN, in) != NULL) {
        lines++;
    }

    // reset in
    fclose(in);
    in = fopen(argv[1], "r");

    // initialize string array
    char strArr[lines][MAX_LEN];
    lines = 0;
    while (fgets(str, MAX_LEN, in) != NULL) {
        for (int i = 0; i < MAX_LEN; i++) {
            strArr[lines][i] = str[i];
        }
        lines++;
    }
    
    // insertion sort
    for (int i = 0; i < lines; i++) {
        moveBack(strList);
        while (index(strList) > -1 &&  strcmp(strArr[i], strArr[get(strList)]) < 0) {
            movePrev(strList);
        }
        if (index(strList) > -1) {
            insertAfter(strList, i);
        }
        else {
            prepend(strList, i);
        }
    }

    // print list to out
    moveFront(strList);
    while (index(strList) > -1) {
        fprintf(out, "%s", strArr[get(strList)]);
        moveNext(strList);
    }

    // close files and free lists
    freeList(&strList);
    strList = NULL;
    fclose(in);
    fclose(out);

    return 0;
}