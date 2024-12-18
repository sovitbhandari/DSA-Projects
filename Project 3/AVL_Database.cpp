/* 
Name: Sovit Bhandari
UID: U83561265
Description: 
             • This file implements an AVL Tree and an Indexed Database using the AVL Tree.
             • The AVL Tree ensures self-balancing during insertions and deletions, maintaining a height of O(log n).
             • The Indexed Database is a wrapper around the AVL Tree, providing additional functionality such as range queries and database clearing.
*/

#include "AVL_Database.hpp"
#include <algorithm>
#include <cmath>

Record::Record(const std::string& k, int v) : key(k), value(v) {}

AVLNode::AVLNode(Record* r) : record(r), left(nullptr), right(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr), nodeCount(0), searchComparisonCount(0) {}

int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}

void AVLTree::updateHeight(AVLNode* node) {
    if (node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

int AVLTree::getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Performs a right rotation on the given node to balance the tree.
AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

// Performs a left rotation on the given node to balance the tree.
AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// Inserts a Record into the AVL Tree.
void AVLTree::insert(Record* record) {
    root = insertHelper(root, record);
}

// Helper function for insertion. Implements the AVL insertion logic and rebalances the tree.
AVLNode* AVLTree::insertHelper(AVLNode* node, Record* record) {
    // Standard BST insertion
    if (!node) {
        nodeCount++;
        return new AVLNode(record);
    }

    // Insert based on record's value
    if (record->value < node->record->value)
        node->left = insertHelper(node->left, record);
    else if (record->value > node->record->value)
        node->right = insertHelper(node->right, record);
    else 
        return node;                                                                    // Duplicate values not allowed

    // Update height of current node
    updateHeight(node);

    // Get balance factor to check if rebalancing is needed
    int balance = getBalance(node);

    // Handle imbalances using rotations
    if (balance > 1 && record->value < node->left->record->value)
        return rotateRight(node);                                                       // Left Left Case

    if (balance < -1 && record->value > node->right->record->value)
        return rotateLeft(node);                                                        // Right Right Case

    if (balance > 1 && record->value > node->left->record->value) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);                                                       // Left Right Case
    }

    if (balance < -1 && record->value < node->right->record->value) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);                                                        // Right Left Case
    }

    return node;
}

// Deletes a node with the given key and value from the AVL Tree.
void AVLTree::deleteNode(const std::string& key, int value) {
    root = deleteHelper(root, key, value);
}

// Helper function for deletion. Implements the AVL deletion logic and rebalances the tree.
AVLNode* AVLTree::deleteHelper(AVLNode* node, const std::string& key, int value) {
    // Standard BST deletion
    if (!node) return nullptr;

    if (value < node->record->value)
        node->left = deleteHelper(node->left, key, value);
    else if (value > node->record->value)
        node->right = deleteHelper(node->right, key, value);
    else {
        // Node with key found
        if (key != node->record->key) {
            if (value < node->record->value)
                node->left = deleteHelper(node->left, key, value);
            else
                node->right = deleteHelper(node->right, key, value);
        } else {
            // Handle cases for deletion
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;                                         // No child case
                }
                else 
                    *node = *temp;                                         // Single child case
                delete temp;
                nodeCount--;
            } else {
                // Two children case
                AVLNode* temp = minValueNode(node->right);
                node->record = temp->record;
                node->right = deleteHelper(node->right, temp->record->key, temp->record->value);
            }
        }
    }

    if (!node) return nullptr;

    // Update height and balance the tree
    updateHeight(node);
    int balance = getBalance(node);

                                                                   // Handle imbalances
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);                                  // Left Left Case

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);                                  // Left Right Case
    }

    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);                                   // Right Right Case

    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);                                   // Right Left Case
    }

    return node;
}

// Finds the node with the minimum value in a subtree.
AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Searches for a Record with the given key and value in the AVL Tree.
Record* AVLTree::search(const std::string& key, int value) {
    searchComparisonCount = 0;
    AVLNode* result = searchHelper(root, key, value);
    return result->record;
}

// Helper function for searching in the AVL Tree.
AVLNode* AVLTree::searchHelper(AVLNode* node, const std::string& key, int value) const {
    searchComparisonCount++;

    if (!node) 
        return new AVLNode(new Record("", 0));

    if (value == node->record->value && key == node->record->key)
        return node;

    if (value < node->record->value)
        return searchHelper(node->left, key, value);
    
    return searchHelper(node->right, key, value);
}

// Provides a database-like interface over the AVL Tree.
void IndexedDatabase::insert(Record* record) {
    index.insert(record);
}

// Searches for a Record in the Indexed Database.
Record* IndexedDatabase::search(const std::string& key, int value) {
    return index.search(key, value);
}

// Deletes a Record from the Indexed Database.
void IndexedDatabase::deleteRecord(const std::string& key, int value) {
    index.deleteNode(key, value);
}

// Helper function for performing range queries recursively.
void IndexedDatabase::rangeQueryHelper(AVLNode* node, int start, int end, 
                                       std::vector<Record*>& result) const {
    if (!node) return;

    if (start <= node->record->value)
        rangeQueryHelper(node->left, start, end, result);

    if (start <= node->record->value && node->record->value <= end)
        result.push_back(node->record);

    if (node->record->value <= end)
        rangeQueryHelper(node->right, start, end, result);
}

std::vector<Record*> IndexedDatabase::rangeQuery(int start, int end) {
    std::vector<Record*> result;
    rangeQueryHelper(index.root, start, end, result);
    return result;
}

void IndexedDatabase::clearHelper(AVLNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node->record;
    delete node;
}

void IndexedDatabase::clearDatabase() {
    clearHelper(index.root);
    index.root = nullptr;
}

int IndexedDatabase::calculateHeight(AVLNode* node) const {
    if (!node) return 0;
    return 1 + std::max(calculateHeight(node->left), calculateHeight(node->right));
}

int IndexedDatabase::getTreeHeight() const {
    return calculateHeight(index.root);
}

int IndexedDatabase::getSearchComparisons(const std::string& key, int value) {
    search(key, value);
    return index.getLastSearchComparisons();
}
