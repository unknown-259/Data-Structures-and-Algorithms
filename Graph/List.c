/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA2
* File: List.c
* Description: Has an underlying structure as a doubly linked list with bi-directional queue 
               that includes a "cursor" to be used for iteration
***/

#include <stdlib.h>
#include "List.h"

// Structs
typedef struct NodeObj* Node;

typedef struct NodeObj {
    int data;
    struct NodeObj* next;
   struct NodeObj* prev;
} NodeObj;

typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;


// Constructors and Destructors
Node newNode(int data) {
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->prev = N->next = NULL;
    return N;
}

void freeNode(Node *pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

List newList(void) {
    List L;
    L = malloc(sizeof(ListObj));
    L->front = L->back = L->cursor = NULL;
    L->index = -1;
    L->length = 0;
    return L;
}

void freeList(List* pL) {
    if (pL != NULL && *pL != NULL) {
        while (length(*pL) != 0) {
            deleteFront(*pL);
        }
        free(*pL);
        *pL = NULL;
    }
}


// Access Functions
int length(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling length() on null List\n");
        exit(EXIT_FAILURE);
    }
    return L->length;
}

int index(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling index() on null List\n");
        exit(EXIT_FAILURE);
    }
    return L->index;
}

// precondition: length > 0
int front(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling front() on null List\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling front() on List length = 0\n");
        exit(EXIT_FAILURE);
    }
    return L->front->data;
}

// precondition: length > 0
int back(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling back() on null List\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling back() on List length = 0\n");
        exit(EXIT_FAILURE);
    }
    return L->back->data;
}

// precondition: length > 0, index >=0
int get(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling get() on null List\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor == NULL) {
        fprintf(stderr, "List Error: calling get() on a List with null cursor\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling get() on List length = 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: calling get() on list index < 0\n");
        exit(EXIT_FAILURE);
    }
    return L->cursor->data;
}

int equals(List A, List B) {
    if (A == NULL || B == NULL) {
        fprintf(stderr, "List Error: calling equals() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (A->length != B->length) {
        return 0;
    }

    Node N = A->front;
    Node M = B->front;
    while (N != NULL) {
        if (N->data != M->data) {
            return 0;
        }
        N = N->next;
        M = M->next;
    }
    return 1;
}


// Manipulation Procedures
void clear(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling clear() on null list\n");
        exit(EXIT_FAILURE);
    }
    while (length(L) != 0) {
        deleteFront(L);
    }
    L->front = L->cursor = L->back = NULL;
    L->index = -1;
    L->length = 0;
}

void set(List L, int x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling set() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling set() on list length = 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: calling set() on list index < 0\n");
        exit(EXIT_FAILURE);
    }
    L->cursor->data = x;
}

void moveFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveFront() on null list\n");
        exit(EXIT_FAILURE);
    }
    L->cursor = L->front;
    if(L->length !=0) {
        L->index = 0;
    }
}

void moveBack(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveBack() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length > 0) {
        L->cursor = L->back;
        L->index = L->length - 1;
    }
}

void movePrev(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling movePrev() on null list\n");
        exit(EXIT_FAILURE);
    }
    if(L->cursor != NULL) {
        L->cursor = L->cursor->prev;
        L->index--;
   }
}

void moveNext(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveNext() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor == L->back) {
        L->cursor = NULL;
        L->index = -1;
    }
    else if (L->cursor != NULL) {
        L->cursor = L->cursor->next;
        L->index++;
    }
}

void prepend(List L, int x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling prepend() on null list\n");
        exit(EXIT_FAILURE);
    }
    Node new = newNode(x);
    if (L->length == 0) {
        L->front = L->back = new;
        L->length++;
    }
    else {
        new->next = L->front;
        L->front->prev = new;
        L->front = new;
        if(L->cursor != NULL) {
            L->index++;
        }
        L->length++;
    }
}

void append(List L, int x) {
    Node new = newNode(x);
    if (L == NULL) {
        fprintf(stderr, "List Error: calling append() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        L->front = L->back = new;
        L->length++;
    }
    else {
        new->prev = L->back;
        L->back->next = new;
        L->back = new;
        L->length++;
    }
}

// precondition: length > 0, index >= 0
void insertBefore(List L, int x) {
    Node N = newNode(x);
    if (L == NULL) {
        fprintf(stderr, "List Error: calling insertBefore() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling insertBefore() on list length = 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: calling equals() on list index < 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL) {
        if (L->cursor == L->front) {
            N->next = L->front;
            L->front->prev = N;
            L->front = N;
        }
        else {
            N->prev = L->cursor->prev;
            N->next = L->cursor;
            L->cursor->prev->next = N;
            L->cursor->prev = N;
        }
        L->index++;
        L->length++;
    }
}

// precondition: length > 0, index >= 0
void insertAfter(List L, int x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling insertAfter() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling insertAfter() on list length = 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: calling insertAfter() on list index < 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL) {
        Node N = newNode(x);
        if (L->cursor == L->back) {
            N->prev = L->back;
            L->back->next = N;
            L->back = N;
        }
        else {
            N->next = L->cursor->next;
            N->prev = L->cursor;
            L->cursor->next->prev = N;
            L->cursor->next = N;
        }
        L->length++;
    }
}

void deleteFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling deleteFront() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling equals() on list length = 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 1) {
        freeNode(&L->front);
        L->front = L->cursor = L->back = NULL;
        L->index = -1;
        L->length = 0;
    }
    else {
        Node N = L->front;
        L->front = L->front->next;
        freeNode(&N);
        L->front->prev = NULL;
        if (L->index == 0) {
            L->cursor = NULL;
        }
        L->index--;
        L->length--;
    }
}

void deleteBack(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling deleteBack() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling deleteBack() on list length = 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 1) {
        freeNode(&L->front);
        L->front = L->cursor = L->back = NULL;
        L->index = -1;
        L->length = 0;
    }
    else {
        Node N = L->back;
        if (L->cursor == N) {
            L->cursor = NULL;
            L->index = -1;
        }
        L->back = L->back->prev;
        freeNode(&N);
        L->back->next = NULL;
        L->length--;
    }    
}

// precondition: length > 0, index >= 0
void delete(List L) {
    Node N = NULL;
    if (L == NULL) {
        fprintf(stderr, "List Error: calling delete() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
        fprintf(stderr, "List Error: calling delete() on list length = 0\n");
        exit(EXIT_FAILURE);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: calling delete() on list index < 0\n");
        exit(EXIT_FAILURE);
    }
    N = L->cursor;
    L->cursor = NULL;
    if (L->length == 1) {
        freeNode(&L->front);
        L->front = L->back = NULL;
    }
    else if (L->front == N) {
        N = L-> front;
        L->front = L->front->next;
        freeNode(&N);
        L->front->prev = NULL;
    }
    else if (L->back == N) {
        N = L->back;
        L->back = L->back->prev;
        freeNode(&N);
        L->back->next = NULL;
    }
    else {
        N->prev->next = N->next;
        N->next->prev = N->prev;
        freeNode(&N);
    }
    L->length--;
    L->index = -1;
}


// Other Operations
void printList(FILE *out, List L) {
    Node N = NULL;
    if (L == NULL) {
        fprintf(stderr, "List Error: calling printList() on null list\n");
        exit(EXIT_FAILURE);
    }
    for (N = L->front; N != NULL; N = N->next){
        fprintf(out, "%d ", N->data);
    }
}

List copyList(List L) {
    Node N = NULL;
    if (L == NULL) {
        fprintf(stderr, "List Error: calling copyList() on null list\n");
        exit(EXIT_FAILURE);
    }
    List copy = newList();
    N = L->front;
    while (N != NULL) {
        append(copy, N->data);
        N = N->next;
    }
    return copy;
}