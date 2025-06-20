/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA4
* File: Sparse.c
* Description: Client for Matrix ADT
***/

#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Incorrect number or arguments\n");
        exit(EXIT_FAILURE);
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Unable to read file\n");
        exit(EXIT_FAILURE);
    }

    FILE* output = fopen(argv[2], "w");
    if (!output) {
        fprintf(stderr, "Unable to open file\n");
        exit(EXIT_FAILURE);
    }

    int n, a, b;
    fscanf(input, "%d %d %d", &n, &a, &b);

    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);

    for (int i = 0; i < a; i++) {
        int row, col;
        double value;
        fscanf(input, "%d %d %lf", &row, &col, &value);
        changeEntry(A, row, col, value);
    }

    for (int i = 0; i < b; i++) {
        int row, col;
        double value;
        fscanf(input, "%d %d %lf", &row, &col, &value);
        changeEntry(B, row, col, value);
    }

    int NNZ_A = NNZ(A);
    int NNZ_B = NNZ(B);

    if (NNZ_A == 0) {
        fprintf(output, "A has no non-zero entries.\n\n");
    } else {
        fprintf(output, "A has %d non-zero entries:\n", NNZ_A);
        printMatrix(output, A);
        fprintf(output, "\n");
    }

    if (NNZ_B == 0) {
        fprintf(output, "B has no non-zero entries.\n\n");
    } else {
        fprintf(output, "B has %d non-zero entries:\n", NNZ_B);
        printMatrix(output, B);
        fprintf(output, "\n");
    }
    
    Matrix result;

    result = scalarMult(1.5, A);
    fprintf(output, "(1.5)*A =\n");
    printMatrix(output, result);
    fprintf(output, "\n");
    freeMatrix(&result);

    result = sum(A, B);
    fprintf(output, "A+B =\n");
    printMatrix(output, result);
    fprintf(output, "\n");
    freeMatrix(&result);

    result = sum(A, A);
    fprintf(output, "A+A =\n");
    printMatrix(output, result);
    fprintf(output, "\n");
    freeMatrix(&result);

    result = diff(B, A);
    fprintf(output, "B-A =\n");
    printMatrix(output, result);
    fprintf(output, "\n");
    freeMatrix(&result);

    result = diff(A, A);
    fprintf(output, "A-A =\n");
    printMatrix(output, result);
    fprintf(output, "\n");
    freeMatrix(&result);

    result = transpose(A);
    fprintf(output, "Transpose(A) =\n");
    printMatrix(output, result);
    fprintf(output, "\n");
    freeMatrix(&result);

    result = product(A, B);
    fprintf(output, "A*B =\n");
    printMatrix(output, result);
    fprintf(output, "\n");
    freeMatrix(&result);

    result = product(B, B);
    fprintf(output, "B*B =\n");
    printMatrix(output, result);
    fprintf(output, "\n");
    freeMatrix(&result);

    freeMatrix(&A);
    freeMatrix(&B);

    fclose(input);
    fclose(output);

    return 0;
}
