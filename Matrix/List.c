/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA4
* File: List.c
* Description: Has an underlying structure as a doubly linked list with bi-directional queue 
               that includes a "cursor" to be used for iteration
***/

#include <stdlib.h>
#include "List.h"

// Structs
typedef struct NodeObj* Node;

typedef struct NodeObj {
    void* data;
    struct NodeObj* next;
   struct NodeObj* previous;
} NodeObj;

typedef struct ListObj {
    int length;
    int index;
    Node front;
    Node back;
    Node cursor;
} ListObj;

// Constructors and Destructors
Node newNode(void* data) {
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->previous = NULL;
    N->next = NULL;
    return N;
}

void freeNode(Node *pN) {
    if (pN != NULL) {
        if (*pN != NULL) {
            free(*pN);
            *pN = NULL;
        }
    } 
}

List newList(void) {
    List L;
    L = malloc(sizeof(ListObj));
    L->front = NULL;
    L->back = NULL;
    L->cursor = NULL;
    L->index = -1;
    L->length = 0;
    return L;
}

void freeList(List* pL) {
    if (pL != NULL) {
        if (*pL != NULL) {
            while (length(*pL) != 0) {
                deleteFront(*pL);
            }
            free(*pL);
            *pL = NULL;
        }
    }
}


// Access Functions
int length(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling length() on null List\n");
        exit(EXIT_FAILURE);
    }
    return L->length;
}

int index(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling index() on null List\n");
        exit(EXIT_FAILURE);
    }
    return L->index;
}

// precondition: length > 0
void* front(List L) {
    if (!L) {
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
void* back(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling back() on null List\n");
        exit(EXIT_FAILURE);
    } else if (L->length == 0) {
        fprintf(stderr, "List Error: calling back() on List length = 0\n");
        exit(EXIT_FAILURE);
    } else {
        return L->back->data;
    }
}

// precondition: length > 0, index >=0
void* get(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling get() on null List\n");
        exit(EXIT_FAILURE);
    } else if (L->length <= 0) {
        fprintf(stderr, "List Error: calling get() on List length <= 0\n");
        exit(EXIT_FAILURE);
    } else if (L->cursor == NULL) {
        fprintf(stderr, "List Error: calling get() on a List with null cursor\n");
        exit(EXIT_FAILURE);
    } else {
        return L->cursor->data;
    }
}

// Manipulation Procedures
void clear(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling clear() on null list\n");
        exit(EXIT_FAILURE);
    }

    while (length(L) != 0) {
        deleteFront(L);
    }

    L->front = NULL;
    L->cursor = NULL;
    L->back = NULL;
    L->length = 0;
    L->index = -1;
}

void set(List L, void* x) {
    if (!L) {
        fprintf(stderr, "List Error: calling set() on null list\n");
        exit(EXIT_FAILURE);
    }else if (L->length <= 0) {
        fprintf(stderr, "List Error: calling set() on list length <= 0\n");
        exit(EXIT_FAILURE);
    } else {
        L->cursor->data = x;
    }
}

void moveFront(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling moveFront() on null list\n");
        exit(EXIT_FAILURE);
    } else {
        L->cursor = L->front;
        if(L->length !=0) {
            L->index = 0;
        }
    }
}

void moveBack(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling moveBack() on null list\n");
        exit(EXIT_FAILURE);
    } else if (L->length > 0) {
        L->cursor = L->back;
        L->index = L->length - 1;
    }
}

void movePrev(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling movePrev() on null list\n");
        exit(EXIT_FAILURE);
    } else if(L->cursor != NULL) {
        L->cursor = L->cursor->previous;
        L->index--;
   }
}

void moveNext(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling moveNext() on null list\n");
        exit(EXIT_FAILURE);
    } else if (L->cursor == L->back) {
        L->cursor = NULL;
        L->index = -1;
    }
    else if (L->cursor != NULL) {
        L->cursor = L->cursor->next;
        L->index++;
    }
}

void prepend(List L, void* x) {
    if (!L) {
        fprintf(stderr, "List Error: calling prepend() on null list\n");
        exit(EXIT_FAILURE);
    }
    Node new = newNode(x);
    if (L->length == 0) {
        L->front = new;
        L->back = new;
        L->length++;
    } else {
        new->next = L->front;
        L->front->previous = new;
        L->front = new;
        if(L->cursor != NULL) {
            L->index++;
        }
        L->length++;
    }
}

void append(List L, void* x) {
    Node new = newNode(x);
    if (!L) {
        fprintf(stderr, "List Error: calling append() on null list\n");
        exit(EXIT_FAILURE);
    } else if (L->length == 0) {
        L->length++;
        L->front = L->back = new;
    } else {
        L->length++;
        new->previous = L->back;
        L->back->next = new;
        L->back = new;
    }
}

void insertBefore(List L, void* x) {
    if (!L) {
        fprintf(stderr, "List Error: calling insertBefore() on null list\n");
        exit(EXIT_FAILURE);
    } else if (L->length <= 0) {
        fprintf(stderr, "List Error: calling insertBefore() on list length <= 0\n");
        exit(EXIT_FAILURE);
    } else if (L->cursor == NULL) {
        fprintf(stderr, "List Error: calling insertBefore() on list NULL cursor\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);
    if (L->cursor == L->front) {
        N->next = L->front;
        L->front->previous = N;
        L->front = N;
    } else {
        N->previous = L->cursor->previous;
        N->next = L->cursor;
        L->cursor->previous->next = N;
        L->cursor->previous = N;
    }
    L->index++;
    L->length++;
}


// precondition: length > 0, index >= 0
void insertAfter(List L, void* x) {
    if (!L) {
        fprintf(stderr, "List Error: calling insertAfter() on null list\n");
        exit(EXIT_FAILURE);
    } else if (L->length <= 0) {
        fprintf(stderr, "List Error: calling insertAfter() on list length <= 0\n");
        exit(EXIT_FAILURE);
    } else if (L->cursor == NULL) {
        fprintf(stderr, "List Error: calling insertBefore() on list NULL cursor\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);
    if (L->cursor == L->back) {
        N->previous = L->back;
        L->back->next = N;
        L->back = N;
    } else {
        N->next = L->cursor->next;
        N->previous = L->cursor;
        L->cursor->next->previous = N;
        L->cursor->next = N;
    }
    L->length++;
}

void deleteFront(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling deleteFront() on null list\n");
        exit(EXIT_FAILURE);
    } else if (L->length == 0) {
        fprintf(stderr, "List Error: calling deleteFront() on list length = 0\n");
        exit(EXIT_FAILURE);
    }

    if (L->length == 1) {
        freeNode(&L->front);
        L->front = L->cursor = L->back = NULL;
        L->index = -1;
        L->length = 0;
    } else { 
        Node N = L->front;
        L->front = L->front->next;
        freeNode(&N);
        L->front->previous = NULL;
        if (L->index == 0) {
            L->cursor = NULL;
        }
        L->index--;
        L->length--;
    }
}


void deleteBack(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling deleteBack() on null list\n");
        exit(EXIT_FAILURE);
    } else if (L->length == 0) {
        fprintf(stderr, "List Error: calling deleteBack() on list length = 0\n");
        exit(EXIT_FAILURE);
    }

    if (L->length == 1) {
        freeNode(&L->front);
        L->front = NULL;
        L->cursor = NULL;
        L->back = NULL;
        L->index = -1;
        L->length = 0;
    } else {
        Node N = L->back;
        if (L->cursor == N) {
            L->cursor = NULL;
            L->index = -1;
        }
        L->back = L->back->previous;
        freeNode(&N);
        L->back->next = NULL;
        L->length--;
    }    
}

// precondition: length > 0, index >= 0
void delete(List L) {
    if (!L) {
        fprintf(stderr, "List Error: calling delete() on null list\n");
        exit(EXIT_FAILURE);
    }
    if (L->length <= 0) {
        fprintf(stderr, "List Error: calling delete() on list length <= 0\n");
        exit(EXIT_FAILURE);
    }

    Node N = L->cursor;
    L->cursor = NULL;

    if (L->length == 1) { // Only one element in the list
        freeNode(&L->front);
        L->front = L->back = NULL;
    } else if (L->front == N) { // Deleting the front element
        L->front = L->front->next;
        freeNode(&N);
        L->front->previous = NULL;
    } else if (L->back == N) { // Deleting the back element
        L->back = L->back->previous;
        freeNode(&N);
        L->back->next = NULL;
    } else { // Deleting an element in the middle
        N->previous->next = N->next;
        N->next->previous = N->previous;
        freeNode(&N);
    }
    L->length--;
    L->index = -1;
}

// Other Operations
void printList(FILE *out, List L) {
    Node N = NULL;
    if (!L) {
        fprintf(stderr, "List Error: calling printList() on null list\n");
        exit(EXIT_FAILURE);
    }
    for (N = L->front; N != NULL; N = N->next){
        fprintf(out, "%p ", N->data);
    }
}
