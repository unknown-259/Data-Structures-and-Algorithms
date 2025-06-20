/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA7
* File: Dictionary.cpp
* Description: Client for Dictionary ADT
***/

#include <stdexcept>
#include "Dictionary.h"

using namespace std;

Dictionary::Node::Node(keyType k, valType v) : key(k), val(v), parent(nullptr), left(nullptr), right(nullptr) {}

Dictionary::Dictionary() : nil(new Node("", 0)), root(nil), current(nil), num_pairs(0) {}

Dictionary::Dictionary(const Dictionary& D) : nil(new Node("", 0)), root(nil), current(nil), num_pairs(0) {
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
    if (R == nil) return;
    s += R->key + "\n";
    preOrderString(s, R->left);
    preOrderString(s, R->right);
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
        throw logic_error("Dictionary: currentKey() current undefined");
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
    Node *node = root, *parent = nil;
    Node* newNode = new Node(k, v);
    newNode->left = newNode->right = nil;

    while (node != nil) {
        parent = node;
        node = (k < node->key) ? node->left : (k > node->key ? node->right : nil);
        if (node == nil) {
            node = parent;
            break;
        }
    }

    if (node == root && root == nil) {
        root = newNode;
    } else if (k < node->key) {
        node->left = newNode;
    } else if (k > node->key) {
        node->right = newNode;
    } else {
        node->val = v;
        delete newNode;
        return;
    }

    newNode->parent = node;
    num_pairs++;
}

void Dictionary::remove(keyType k) {
    Node* node = search(root, k);
    if (node == nil) throw logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    if (node == current) current = nil;

    if (node->left == nil || node->right == nil) {
        Node* replacement = (node->left != nil) ? node->left : node->right;
        if (node == root) root = replacement;
        else if (node == node->parent->left) node->parent->left = replacement;
        else node->parent->right = replacement;
        if (replacement != nil) replacement->parent = node->parent;
        delete node;
    } else {
        Node* successor = findMin(node->right);
        node->key = successor->key;
        node->val = successor->val;
        if (successor->parent->left == successor) successor->parent->left = successor->right;
        else successor->parent->right = successor->right;
        if (successor->right != nil) successor->right->parent = successor->parent;
        delete successor;
    }
    num_pairs--;
}


void Dictionary::begin() {
    current = (root != nil) ? findMin(root) : nil;
}

void Dictionary::end() {
    current = (root != nil) ? findMax(root) : nil;
}

void Dictionary::next() {
    if (current == nil) throw logic_error("Dictionary: next(): current undefined");
    current = findNext(current);
}

void Dictionary::prev() {
    if (current == nil) throw logic_error("Dictionary: prev(): current undefined");
    current = findPrev(current);
}


// Other function
string Dictionary::to_string() const {
    string result;
    inOrderString(result, root);
    return result;
}

string Dictionary::pre_string() const {
    string result;
    preOrderString(result, root);
    return result;
}

bool Dictionary::equals(const Dictionary& D) const {
    return (num_pairs == D.num_pairs) && (to_string() == D.to_string());
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
