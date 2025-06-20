/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA6
* File: List.cpp
* Description: List ADT
***/

#include<stdexcept>
#include "List.h"

using namespace std;

// Constructors & Destructors

List::Node::Node(ListElement val) {
   prev = nullptr;
   next = nullptr;
   data = val;
}

// Create new List
List::List() {
   pos_cursor = 0;
   num_elements = 0;
   backDummy = new Node(0);
   frontDummy = new Node(0);
   frontDummy->next = afterCursor = backDummy;
   backDummy->prev = beforeCursor = frontDummy;
}

List::List(const List& L) {
   pos_cursor = 0;
   num_elements = 0;
   backDummy = new Node(0);
   frontDummy = new Node(0);
   frontDummy->next = backDummy;
   backDummy->prev = frontDummy;
   beforeCursor = frontDummy;
   afterCursor = backDummy;

   for (Node* N = L.frontDummy->next; N != L.backDummy; N = N->next) {
      insertAfter(N->data);
      beforeCursor = beforeCursor->next;
      afterCursor = backDummy;
   }

   moveFront();
}

// Destructor
List::~List() {
   clear();
   delete backDummy;
   delete frontDummy;
}

// Access functions

int List::length() const {
   return num_elements;
}

ListElement List::front() const {
   if (length() <= 0) {
      throw length_error("List error: calling front() on empty list\n");
   } else {
      return frontDummy->next->data;
   }
}

ListElement List::back() const {
   if (length() <= 0) {
      throw length_error("Lis error: calling back() on empty list\n");
   } else {
      return backDummy->prev->data;
   }
}

int List::position() const {
   if (pos_cursor < 0) {
      throw range_error("List error: calling position() on empty list\n");
   } else if (pos_cursor > length()) {
      throw range_error("List error: calling position() on empty list\n");
   } else {
      return pos_cursor;
   }
}

// Returns the element after the cursor.
ListElement List::peekNext() const {
   if (pos_cursor >= length()) {
      throw range_error("List error: calling peekNext() cursor at back\n");
   } else {
      return afterCursor->data;
   }
}

ListElement List::peekPrev() const {
   if (pos_cursor <= 0) {
      throw range_error("List error: calling peekPrev() cursor at front\n");
   } else {
      return beforeCursor->data;
   }
}

// Manipulation procedures

void List::clear() {
    while (length() > 0) {
        moveFront();
        eraseAfter();
    }
}

void List::moveFront(){
   pos_cursor = 0;
   afterCursor = frontDummy->next;
   beforeCursor = frontDummy;
}

void List::moveBack(){
   pos_cursor = num_elements;
   afterCursor = backDummy;
   beforeCursor = backDummy->prev;
}

ListElement List::moveNext() {
   if (afterCursor == nullptr) {
      throw range_error("List error: calling moveNext() cursor at end of list\n");
   } else if (beforeCursor == nullptr) {
      throw range_error("List error: calling moveNext() cursor at front\n");
   } else {
      beforeCursor = afterCursor;
      afterCursor = afterCursor->next;
      pos_cursor++;
      return beforeCursor->data;
   }
}

// Advances cursor to next lower position. Returns the List element that
ListElement List::movePrev() {
    if (position() < 0) {
      throw range_error("List error: calling movePrev() cursor at front\n");
    }
    afterCursor = afterCursor->prev;
    beforeCursor = beforeCursor->prev;
    pos_cursor--;
    return afterCursor->data;
}

void List::insertAfter(ListElement x) {
    if (beforeCursor == nullptr) {
        throw range_error("List error: calling insertAfter() cursor at front\n");
    }
    if (afterCursor == nullptr && length() > 0) {
        throw range_error("List error: callng insertAfter() cursor at end\n");
    }

    Node *temp = new Node(x);
    temp->prev = beforeCursor;
    temp->next = afterCursor;
    beforeCursor->next = temp;
   
    if (afterCursor != nullptr) {
        afterCursor->prev = temp;
    }

    afterCursor = temp; 
    num_elements++;
}

void List::insertBefore(ListElement x) {
    if (afterCursor == nullptr) {
      exit(EXIT_FAILURE);
    } else if (beforeCursor == nullptr) {
      exit(EXIT_FAILURE);
    } else if (pos_cursor < 0) {
      exit(EXIT_FAILURE);
    }

    Node *temp = new Node(x);

    temp->next = afterCursor;
    afterCursor->prev = temp;
    temp->prev = beforeCursor;
    beforeCursor->next = temp;
    beforeCursor = temp;

    num_elements++;
    pos_cursor++;
}

void List::setAfter(ListElement x) {
    if (pos_cursor >= length()) {
      throw range_error("List error: calling setAfter() cursor at back\n");
   }
   afterCursor->next->data = x;
}

void List::setBefore(ListElement x) {
   if (pos_cursor <= 0) {
      throw range_error("List Error: calling setBefore() cursor at front\n");
   }
   beforeCursor->prev->data = x;
}

void List::eraseAfter() {
   if (afterCursor == nullptr) {
      throw range_error("List Error: calling eraseAfter() cursor at back\n");
   } else {
      Node *N = afterCursor;

      beforeCursor->next = afterCursor->next;
      if (afterCursor->next != nullptr) {
         afterCursor->next->prev = beforeCursor;
      }

      afterCursor = afterCursor->next;
      delete N; 
      num_elements--;
   }
}

void List::eraseBefore() {
    if (beforeCursor == nullptr) {
        throw range_error("List Error: calling eraseBefore() cursor at front\n");
    }

    Node* temp = beforeCursor;
    beforeCursor = beforeCursor->prev;

    if (beforeCursor != nullptr) {
        beforeCursor->next = afterCursor;
    } else {
        afterCursor->prev = nullptr;
    }

    delete temp;
    num_elements--;
    pos_cursor--;
}


// Other Functions

int List::findNext(ListElement x) {
    Node* N = afterCursor;
    
    if (N == nullptr) {
        return -1;
    }

    for (int i = pos_cursor; N != nullptr; i++) {
        if (N->data == x) {
            afterCursor = N->next;
            beforeCursor = N;
            pos_cursor = i + 1;
            return pos_cursor;
        } else {
         N = N->next;
        }
    }

    afterCursor = nullptr;
    beforeCursor = backDummy->prev;
    pos_cursor = num_elements;
    return -1;
}

int List::findPrev(ListElement x) {
    Node* temp = afterCursor;
    int prevPos = pos_cursor - 1;

    while (temp != nullptr) {
        if (temp == frontDummy) break;

        if (temp->data == x) {
            pos_cursor = prevPos;
            afterCursor = temp;
            beforeCursor = temp->prev;
            return prevPos;
        }

        temp = temp->prev;
        prevPos--;
    }

    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
    return -1;
}

void List::cleanup() {
   int position = 0;
   Node* cursor = frontDummy->next;

   while (cursor != backDummy) {
      Node* current = cursor;
      Node* n = cursor->next;
      int inner_position = position + 1;

      while (n != backDummy) {
         if (cursor->data == n->data) {
            current->next = n->next;
            if (n->next != nullptr) {
               n->next->prev = current;
            }
            delete n;
            n = current->next;
            num_elements--;
            if (inner_position <= pos_cursor) {
               pos_cursor--;
            }
         } else {
            current = n;
            n = current->next;
         }
         inner_position++;
      }
      position++;
      cursor = cursor->next;
   }
}

List List::concat(const List& L) const {
   List newList;
   Node* M = L.frontDummy->next;
   Node* N = frontDummy->next;

   while (N != backDummy) {
      newList.insertAfter(N->data);
      N = N->next;
   }

   while (M != L.backDummy) {
      newList.insertAfter(M->data);
      M = M->next;
   }

   return newList;
}

std::string List::to_string() const {
   std::string s;
   for (Node* N = frontDummy; N != nullptr; N = N->next) {
      s += std::to_string(N->data) + " ";
   }
   return s;
}

bool List::equals(const List& R) const {
   if (num_elements != R.num_elements) {
      return false;
   }
   Node* M = R.frontDummy;
   Node* N = frontDummy;
   while (N != nullptr && M != nullptr) {
      if (N->data != M->data) {
         return false;
      }
      M = M->next;
      N = N->next;
   }
   return true;
}


// Overriden Operators

std::ostream& operator<<(std::ostream& stream, const List& L) {
    for (List::Node* current = L.frontDummy->next; current != L.backDummy; current = current->next) {
        stream << current->data;
        if (current->next != L.backDummy) {
            stream << " ";
        }
    }
    return stream;
}

bool operator==( const List& A, const List& B ) {
   return A.List::equals(B);
} 

List& List::operator=(const List& L) {
    if (this != &L) {
      List temp = L;
      swap(frontDummy, temp.frontDummy);
      swap(backDummy, temp.backDummy);
      swap(num_elements, temp.num_elements);
      swap(beforeCursor, temp.beforeCursor);
      swap(afterCursor, temp.afterCursor);
      swap(pos_cursor, temp.pos_cursor);
    }
    return *this;
}