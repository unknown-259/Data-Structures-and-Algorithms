/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA4
* File: List.h
* Description: Has an underlying structure as a doubly linked list with bi-directional queue 
               that includes a "cursor" to be used for iteration
***/

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>

typedef struct ListObj* List;

List newList(void);        // Creates and returns a new empty List
void freeList(List* pL);   // Frees all heap memory associated with *pL

int length(List L);        // Returns the number of elements in L
int index(List L);         // Returns the index of the cursor element
void* front(List L);         // Returns front element (precondition: length() > 0)
void* back(List L);          // Returns back element (precondition: length() > 0)
void* get(List L);           // Returns the cursor element (precondition: length() > 0, index() >= 0)

void clear(List L);        // Resets L to its original empty state
void set(List L, void* x);   // Overwrites cursor element with x
void moveFront(List L);    // Moves cursor to front
void moveBack(List L);     // Moves cursor to back
void movePrev(List L);     // Moves cursor one step toward front
void moveNext(List L);     // Moves cursor one step toward back
void prepend(List L, void* x); // Inserts new element before front
void append(List L, void* x);  // Inserts new element after back
void insertBefore(List L, void* x); // Inserts element before cursor
void insertAfter(List L, void* x);  // Inserts element after cursor
void deleteFront(List L);  // Deletes front element
void deleteBack(List L);   // Deletes back element
void delete(List L);       // Deletes cursor element


void printList(FILE* out, List L);  // Prints a space-separated list to file

#endif
