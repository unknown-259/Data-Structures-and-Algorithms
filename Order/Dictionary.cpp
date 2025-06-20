/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA8
* File: Dictionary.cpp
* Description: Client for Dictionary ADT
***/

#include <stdexcept>
#include "Dictionary.h"

#define RED -1
#define BLACK -2

using namespace std;

// Dictionary::Node::Node(keyType k, valType v) : key(k), val(v), parent(nullptr), left(nullptr), right(nullptr) {}

// Dictionary::Dictionary() : nil(new Node("", 0)), root(nil), current(nil), num_pairs(0) {}

// Dictionary::Dictionary(const Dictionary& D) : nil(new Node("", 0)), root(nil), current(nil), num_pairs(0) {
//     preOrderCopy(D.root, D.nil);
// }

// Dictionary::~Dictionary() {
//     postOrderDelete(root);
//     delete nil;
// }

//Constructors & Destructors

Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = RED;
}

Dictionary::Dictionary() {
    nil = new Node("\0", -1);
    nil->color = BLACK;
    current = nil;
    root = nil;
    num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary& D) {
    nil = new Node("\0", -1);
    nil->color = BLACK;
    root = nil;
    num_pairs = 0;
    current = nil;
    preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary() {
    postOrderDelete(root);
    delete nil;
}

// Helper functions
void Dictionary::inOrderString(std::string& s, Node* R) const {
    if (R == nil) return;
    inOrderString(s, R->left);
    s += R->key + " : " + std::to_string(R->val) + "\n";
    inOrderString(s, R->right);
}

void Dictionary::preOrderString(std::string& s, Node* R) const {
    if (R != nil) {
        s += R->key;
        if (R->color == RED) {
            s += " (RED)";
        }
        s += "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

void Dictionary::preOrderCopy(Node* R, Node* N) {
    if (R == N) return;
    setValue(R->key, R->val);
    preOrderCopy(R->left, N);
    preOrderCopy(R->right, N);
}

void Dictionary::postOrderDelete(Node* R) {
    if (R == nil) return;
    postOrderDelete(R->left);
    postOrderDelete(R->right);
    delete R;
}

Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    while (R != nil && k != R->key) {
        R = (k < R->key) ? R->left : R->right;
    }
    return R;
}

Dictionary::Node* Dictionary::findMin(Node* R) {
    while (R && R->left != nil) {
        R = R->left;
    }
    return R;
}

Dictionary::Node* Dictionary::findMax(Node* R) {
    if (R == nil) return nil;
    while (R->right != nil) {
        R = R->right;
    }
    return R;
}

Dictionary::Node* Dictionary::findNext(Node* N) {
    if (N == nil) return nil;
    if (N->right != nil) {
        return findMin(N->right);
    }
    Node* P = N->parent;
    while (P != nil && N == P->right) {
        N = P;
        P = P->parent;
    }
    return P;
}

Dictionary::Node* Dictionary::findPrev(Node* N) {
    if (N == nil) return nil;
    if (N->left != nil) {
        return findMax(N->left);
    }
    Node* P = N->parent;
    while (P != nil && N == P->left) {
        N = P;
        P = P->parent;
    }
    return P;
}

// Red Black Tree Helper Functions
void Dictionary::LeftRotate(Node* N) {
    Node* newRoot = N->right;
    Node* subtreeLeft = newRoot->left;

    newRoot->left = N;
    N->right = subtreeLeft;

    if (subtreeLeft != nil) {
        subtreeLeft->parent = N;
    }

    newRoot->parent = N->parent;
    if (N->parent == nil) {
        root = newRoot;
    } else if (N == N->parent->left) {
        N->parent->left = newRoot;
    } else {
        N->parent->right = newRoot;
    }

    N->parent = newRoot;
}

void Dictionary::RightRotate(Node* N) {
    Node* newRoot = N->left;
    Node* subtreeRight = newRoot->right;

    newRoot->right = N;
    N->left = subtreeRight;

    if (subtreeRight != nil) {
        subtreeRight->parent = N;
    }

    newRoot->parent = N->parent;
    if (N->parent == nil) {
        root = newRoot;
    } else if (N == N->parent->right) {
        N->parent->right = newRoot;
    } else {
        N->parent->left = newRoot;
    }

    N->parent = newRoot;
}

void Dictionary::RB_InsertFixUp(Node* N) {
    while (N->parent->color == RED) {
        if (N->parent == N->parent->parent->left) {
            Node* sibling = N->parent->parent->right;
            if (sibling->color == RED) {
                N->parent->color = BLACK;
                sibling->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else if (N == N->parent->right) {
                N = N->parent;
                LeftRotate(N);
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                RightRotate(N->parent->parent);
            } else {
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                RightRotate(N->parent->parent);
            }
        } else {
            Node* sibling = N->parent->parent->left;
            if (sibling->color == RED) {
                N->parent->color = BLACK;
                sibling->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else if (N == N->parent->left) {
                N = N->parent;
                RightRotate(N);
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                LeftRotate(N->parent->parent);
            } else {
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                LeftRotate(N->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void Dictionary::RB_Transplant(Node* u, Node* v) {
    if (u->parent == nil) {
        root = v;
    } else if (u->parent->left == u) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nil) {
        v->parent = u->parent;
    }
}

void Dictionary::RB_DeleteFixUp(Node* N) {
    while (N != root && N->color == BLACK) {
        if (N == N->parent->left) {
            Node* Sibling = N->parent->right;
            if (Sibling->color == RED) {
                Sibling->color = BLACK;
                N->parent->color = RED;
                LeftRotate(N->parent);
                Sibling = N->parent->right;
            }
            if (Sibling->left->color == BLACK && Sibling->right->color == BLACK) {
                Sibling->color = RED;
                N = N->parent;
            } else if (Sibling->right->color == BLACK) {
                Sibling->left->color = BLACK;
                Sibling->color = RED;
                RightRotate(Sibling);
                Sibling = N->parent->right;
                Sibling->color = N->parent->color;
                N->parent->color = BLACK;
                Sibling->right->color = BLACK;
                LeftRotate(N->parent);
                N = root;
            } else {
                Sibling->color = N->parent->color;
                N->parent->color = BLACK;
                Sibling->right->color = BLACK;
                LeftRotate(N->parent);
                N = root;
            }
        } else {
            Node* Sibling = N->parent->left;
            if (Sibling->color == RED) {
                Sibling->color = BLACK;
                N->parent->color = RED;
                RightRotate(N->parent);
                Sibling = N->parent->left;
            }
            if (Sibling->right->color == BLACK && Sibling->left->color == BLACK) {
                Sibling->color = RED;
                N = N->parent;
            } else if (Sibling->left->color == BLACK) {
                Sibling->right->color = BLACK;
                Sibling->color = RED;
                LeftRotate(Sibling);
                Sibling = N->parent->left;
                Sibling->color = N->parent->color;
                N->parent->color = BLACK;
                Sibling->left->color = BLACK;
                RightRotate(N->parent);
                N = root;
            } else {
                Sibling->color = N->parent->color;
                N->parent->color = BLACK;
                Sibling->left->color = BLACK;
                RightRotate(N->parent);
                N = root;
            }
        }
    }
    if (N != nil) {
        N->color = BLACK;
    }
}

void Dictionary::RB_Delete(Node* N) {
    Node* substitute;
    Node* target = N;
    bool wasBlack = target->color;
    if (target->left == nil) {
        substitute = target->right;
        RB_Transplant(target, target->right);
    } else if (target->right == nil) {
        substitute = target->left;
        RB_Transplant(target, target->left);
    } else {
        target = findMin(target->right);
        wasBlack = target->color;
        substitute = target->right;
        if (target->parent != N) {
            RB_Transplant(target, target->right);
            target->right = N->right;
            if (target->right != nil) {
                target->right->parent = target;
            }
        }
        RB_Transplant(N, target);
        target->left = N->left;
        target->left->parent = target;
        target->color = N->color;
    }

    if (wasBlack && substitute != nil) {
        RB_DeleteFixUp(substitute);
    }

    delete N;
}


// Access functions
int Dictionary::size() const {
    return num_pairs > 0 ? num_pairs : 0;
}

bool Dictionary::contains(keyType k) const {
    return (search(root, k) != nil) ? true : false;
}

valType& Dictionary::getValue(keyType k) const {
    Node* foundNode = search(root, k);
    if (foundNode == nil) {
        throw logic_error("Dictionary: getValue(): key \"" + k + "\" does not exist");
    }
    return foundNode->val;
}

bool Dictionary::hasCurrent() const {
    return current == nil ? false : true;
}

keyType Dictionary::currentKey() const {
    if (current == nil) {
        throw logic_error("Dictionary: currentKey() current undefinedn");
    }
    return current->key;
}

valType& Dictionary::currentVal() const {
    if (current == nil) {
        throw logic_error("Dictionary: current undefined");
    }
    return current->val;
}

// Manipulation procedure 
void Dictionary::clear() {
    postOrderDelete(root);
    num_pairs = 0;
    current = nil;
    root = nil;
}

void Dictionary::setValue(keyType k, valType v) {
    Node* current = root;
    Node* parent = nil;
    
    while (current != nil) {
        parent = current;

        // Traverse left or right depending on comparison
        if (k < current->key) {
            current = current->left;
        } else if (k > current->key) {
            current = current->right;
        } else {
            // Key found, directly update the value and return
            current->val = v;
            return;
        }
    }

    // Node doesn't exist; create and set it
    Node* newNode = new Node(k, v);
    newNode->left = nil;
    newNode->right = nil;
    newNode->color = RED;
    newNode->parent = parent;

    if (parent == nil) {
        root = newNode; // If no nodes exist, this is the root
    } else if (k < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    num_pairs++;
    RB_InsertFixUp(newNode);
}

void Dictionary::remove(keyType k) {
    Node* target = search(root, k);
    if (target == nil) {
        throw logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    }

    if (current == target) {
        current = nil;
    }

    Node* replacement = search(root, k);
    RB_Delete(replacement);
    num_pairs--;
} 

void Dictionary::begin() {
    if (num_pairs > 0) {
        current = findMin(root);
    }
}

void Dictionary::end() {
    if (num_pairs > 0) {
        current = findMax(root);
    }
}

void Dictionary::next() {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: next(): current undefined");
    }

    Node* successor = findNext(current);
    current = (successor != nil) ? successor : nil;
}

void Dictionary::prev() {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: prev(): current undefined");
    }

    Node* predecessor = findPrev(current);
    current = (predecessor != nil) ? predecessor : nil;
}
 
// Other function
std::string Dictionary::to_string() const {
    string S = "";
    this->inOrderString(S, this->root);
    return S;
}

std::string Dictionary::pre_string() const {
    string S = "";
    this->preOrderString(S, this->root);
    return S;
}

bool Dictionary::equals(const Dictionary& D) const {
    return num_pairs == D.num_pairs && to_string() == D.to_string();
}

// Overloaded operator
ostream& operator<<(ostream& stream, Dictionary& D) {
    return stream << D.to_string();
}

bool operator==(const Dictionary& A, const Dictionary& B) {
    return A.equals(B);
}

Dictionary& Dictionary::operator=(const Dictionary& D) {
    if (this != &D) {
        postOrderDelete(root);
        root = nil;
        num_pairs = 0;
        preOrderCopy(D.root, D.nil);
    }
    return *this;
}
