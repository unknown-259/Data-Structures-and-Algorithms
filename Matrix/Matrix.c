/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA4
* File: Matrix.c
* Description: Matrix ADT
***/

#include <stdio.h>
#include <stdlib.h>

#include "Matrix.h"
#include "List.h"

typedef struct EntryObj {
   int column;
   double val;
} EntryObj;

typedef struct MatrixObj {
   int size;
   int NNZ;
   List* entry;
} MatrixObj;

typedef struct EntryObj *Entry;

// constructor and destructor
Entry newEntry(int column, double val) {
   Entry E = malloc(sizeof(EntryObj));
   E->column = column;
   E->val = val;
   return E;
}

void freeEntry(Entry* pE) {
   if (!pE){
    if (!*pE) {
        free(*pE);
        *pE = NULL;
    }
   }
}

// Constructor & destructor
Matrix newMatrix(int n) {
    if (n <= 0) {
        fprintf(stderr, "Matrix Error: newMatrix() called with non-positive size\n");
        exit(EXIT_FAILURE);
    } else {
        Matrix M = malloc(sizeof(MatrixObj));
        M->NNZ = 0;
        M->size = n;
        M->entry = calloc(n+1, sizeof(List));
        for (int i = 1; i <= n; i++) {
            M->entry[i] = newList();
            if (M->entry[i] == NULL) {
                fprintf(stderr, "Matrix Error: newList() failed during row initialization in newMatrix()\n");
                for (int j = 1; j < i; j++) {
                    freeList(&(M->entry[j]));
                }
                free(M->entry);
                free(M);
                exit(EXIT_FAILURE);
            }
        }
        return M;
    }
}

void freeMatrix(Matrix* pM) {
    if (pM != NULL) {
        if (*pM != NULL) {
            makeZero(*pM);
            for (int i = 1; i <= (*pM)->size; i++) {
                freeList(&((*pM)->entry[i]));
            }
            free((*pM)->entry);
            free(*pM);
            *pM = NULL;
        }
    }
}

// Access functions

int size(Matrix M) {
   return M->size;
}

int NNZ(Matrix M) {
   return M->NNZ;
}

int equals(Matrix A, Matrix B) {
    if (!A) {
        fprintf(stderr, "Matrix Error: calling equals() on NULL matrix\n");
        exit(EXIT_FAILURE);
    } else if (B == NULL) {
        fprintf(stderr, "Matrix Error: calling equals() on NULL matrix\n");
        exit(EXIT_FAILURE);
    } else if (size(A) != size(B)) {
        return 0;
    } else {
        for (int i = 1; i <= A->size; i++) {
            List rowA = A->entry[i];
            List rowB = B->entry[i];

            if (length(rowA) != length(rowB)) {
                return 0;
            }

            moveFront(rowA);
            moveFront(rowB);
            while (index(rowA) >= 0) {
                Entry* entryA = (Entry*)get(rowA);
                Entry* entryB = (Entry*)get(rowB);
                if (((Entry)get(A->entry[i]))->column != ((Entry)get(B->entry[i]))->column || ((Entry)get(A->entry[i]))->val != ((Entry)get(A->entry[i]))->val) {
                    return 0;
                }
                moveNext(rowA);
                moveNext(rowB);
            }
        }
        return 1;
    }
}

// Manipulation procedures

void makeZero(Matrix M) {
    for (int i = 1; i <= size(M); i++) {
        List row = M->entry[i];
        moveFront(row);
        while (index(row) != -1) {
            Entry cursor = get(row);
            freeEntry(&cursor);
            moveNext(row);
        }
        clear(M->entry[i]);
    }
    M->NNZ = 0;
}

void changeEntry(Matrix M, int i, int j, double x) {
   if (!M) {
        fprintf(stderr, "Matrix Error: calling changeEntry() on NULL matrix\n");
        exit(EXIT_FAILURE);
   } else if (i < 1 || i > M->size) {
        fprintf(stderr, "Matrix Error: calling changeEntry() with index not in range\n");
        exit(EXIT_FAILURE);
   } else if (j < 1 || j > M->size) {
        fprintf(stderr, "Matrix Error: calling changeEntry() with index not in range\n");
        exit(EXIT_FAILURE);
   }

    List row = M->entry[i];
    moveFront(row);
    while (index(row) >= 0) {
        Entry E = (Entry)get(row);
        
        if (E->column == j) {
            if (x == 0.0) {
                freeEntry(&E);
                delete(row);
                M->NNZ--;
            } else {
                E->val = x;
            }
            return;
        }
        
        if (E->column > j) {
            if (x != 0.0) {
                insertBefore(row, newEntry(j, x));
                M->NNZ++;
            }
            return;
        }
        moveNext(row);
    }
    
    if (x != 0.0) {
        append(row, newEntry(j, x));
        M->NNZ++;
    }
}

// Matrix Arithmetic operations

Matrix copy(Matrix A) {
    if (!A) {
        fprintf(stderr, "Matrix Error: copy() called on NULL matrix\n");
        exit(EXIT_FAILURE);
    }

   Matrix B = newMatrix(A->size);
   for (int i = 1; i <= A->size; i++) {
      List row = A->entry[i];
      moveFront(row);
      while (index(row) != -1) {
         Entry entry = (Entry) get(row);
         changeEntry(B, i, entry->column, entry->val);
         moveNext(row);
      }
   }
   return B;
}

// Returns a reference to a new Matrix object representing the transpose of A
Matrix transpose(Matrix A) {
    if (!A) {
        fprintf(stderr, "Matrix Error: transpose() called on NULL matrix\n");
        exit(EXIT_FAILURE);
    }

   Matrix B = newMatrix(A->size);
   for (int i = 1; i <= A->size; i++) {
        List rowA = A->entry[i];
        moveFront(rowA);
        while (index(rowA) >= 0) {
            Entry entryA = (Entry)get(rowA);
            append(B->entry[entryA->column], newEntry(i, entryA->val));
            moveNext(rowA);
        }
    }
    B->NNZ = A->NNZ;
    return B;
}

// Returns a reference to a new Matrix object representing xA
Matrix scalarMult(double x, Matrix A) {
    if (!A) {
        fprintf(stderr, "Matrix Error: scalarMult() called on NULL matrix\n");
        exit(EXIT_FAILURE);
    }

   Matrix B = newMatrix(A->size);
   for (int i = 1; i <= A->size; i++) {
        List rowA = A->entry[i];
        moveFront(rowA);

        while (index(rowA) >= 0) {
            Entry entryA = (Entry)get(rowA);
            double scaledValue = x * entryA->val;
            if (scaledValue != 0.0) {
                append(B->entry[i], newEntry(entryA->column, scaledValue));
            }

            moveNext(rowA);
        }
    }
    B->NNZ = (x == 0.0) ? 0 : A->NNZ;
    return B;
}

Matrix sum(Matrix A, Matrix B) {
    if (!A) {
        fprintf(stderr, "Matrix Error: calling sum() on NULL matrix\n");
        exit(EXIT_FAILURE);
    } else if (!B) {
        fprintf(stderr, "Matrix Error: calling sum() on NULL matrix\n");
        exit(EXIT_FAILURE);
    } else if (size(A) != size(B)) {
        fprintf(stderr, "Matrix Error: calling sum() on different sized matricies");
        exit(EXIT_FAILURE);
    } else if (A == B) {
        return scalarMult(2, A);
    }
    
    Matrix C = newMatrix(size(A));
    for (int i = 1; i <= size(A); i++) {
        List rowA = A->entry[i];
        List rowB = B->entry[i];
        List rowC = C->entry[i];

        moveFront(rowA);
        moveFront(rowB);

        while (index(rowA) >= 0 || index(rowB) >= 0) {
            Entry aE = (index(rowA) >= 0) ? (Entry)get(rowA) : NULL;
            Entry bE = (index(rowB) >= 0) ? (Entry)get(rowB) : NULL;

            if (aE && (!bE || aE->column < bE->column)) {
                append(rowC, newEntry(aE->column, aE->val));
                C->NNZ++;
                moveNext(rowA);
            } else if (bE && (!aE || bE->column < aE->column)) {
                append(rowC, newEntry(bE->column, bE->val));
                C->NNZ++;
                moveNext(rowB);
            } else if (aE && bE && aE->column == bE->column) {
                double sumVal = aE->val + bE->val;
                if (sumVal != 0.0) {
                    append(rowC, newEntry(aE->column, sumVal));
                    C->NNZ++;
                }
                moveNext(rowA);
                moveNext(rowB);
            }
        }
    }
    return C;
}

Matrix diff(Matrix A, Matrix B) {
    if (A->size != B->size) {
        fprintf(stderr, "Matrix Error: calling diff() on matrices of different sizes\n");
        exit(EXIT_FAILURE);
    }
    Matrix negativeB = scalarMult(-1.0, B);
    Matrix result = sum(A, negativeB);
    freeMatrix(&negativeB);
    return result;
}

double dot(List A, List B) {
    if (length(A) == 0 || length(B) == 0) {
        return 0.0;
    }
    
    double dot_prod = 0.0;
    moveFront(A);
    moveFront(B);
    while (index(A) >= 0 && index(B) >= 0) {
        Entry eA = (Entry)get(A);
        Entry eB = (Entry)get(B);
        if (eA->column == eB->column) {
            dot_prod += eA->val * eB->val;
            moveNext(A);
            moveNext(B);
        } else if (eA->column < eB->column) {
            moveNext(A);
        } else {
            moveNext(B);
        }
    }
    return dot_prod;
}

Matrix product(Matrix A, Matrix B) {
    if (!A || !B) {
        fprintf(stderr, "Matrix Error: calling product() on NULL Matrix\n");
        exit(EXIT_FAILURE);
    } else if (A->size != B->size) {
        fprintf(stderr, "Matrix Error: calling product() on matrices of different sizes\n");
        exit(EXIT_FAILURE);
    }

    Matrix P = newMatrix(A->size);
    if (A->NNZ == 0) {
        return P;
    } else if (B->NNZ == 0) {
        return P;
    }

     Matrix BT = transpose(B);

    // Iterate over each row in matrix A
    for (int i = 1; i <= A->size; i++) {
        // Get the current row in A
        List Prow = A->entry[i];
        
        // Process only non-empty rows in A
        if (length(Prow) > 0) {
            // Iterate over each column in matrix B (which corresponds to rows in BT)
            for (int j = 1; j <= BT->size; j++) {
                List Qrow = BT->entry[j];  // Get the j-th column of B (i.e., the j-th row of BT)

                // Process only non-empty rows in BT (columns in B)
                if (length(Qrow) > 0) {
                    // Compute the dot product of row i in A and column j in B
                    double dp = dot(Prow, Qrow);
                    
                    // If the dot product is non-zero, append the result to matrix P
                    if (dp != 0) {
                        Entry e = newEntry(j, dp);  // Create a new entry with the column index and the dot product value
                        append(P->entry[i], e);    // Append the entry to the i-th row of matrix P
                        P->NNZ++;                  // Increment the number of non-zero entries in P    
                    }
                }
            }
        }
    }
    freeMatrix(&BT);
    return P;
}

void printMatrix(FILE* out, Matrix M) {
   if (!out) {
        printf("Matrix Error: calling printMatrix() on NULL parameters\n");
        exit(EXIT_FAILURE);
   } else if (!M) {
        printf("Matrix Error: calling printMatrix() on NULL parameters\n");
        exit(EXIT_FAILURE);
   } else {
        for (int n = 1; n <= M->size; n++) {
            List row = M->entry[n];
            
            if (length(row) > 0) {
                fprintf(out, "%d: ", n);
                for (moveFront(row); index(row) >= 0; moveNext(row)) {
                    Entry e = (Entry)get(row);
                    fprintf(out, "(%d, %.1f) ", e->column, e->val);
                }
                fprintf(out, "\n");
            }
        }
   }
}