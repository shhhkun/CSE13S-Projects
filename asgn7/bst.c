#include "bst.h"
#include "node.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

uint64_t branches; // extern var

// Design based off lecture 18 (Trees and BST's) by Prof. Long.
//
// Usage: This function takes two numbers a and b and returns the larger of the two.
//
// Parameters: numbers a and b.
// Return value: the larger number between a and b.
static uint32_t max(uint32_t a, uint32_t b) {
    return a > b ? a : b;
}

// Usage: Constructor for a binary search tree, the root is initially NULL.
//
// Parameters: void.
// Return value: NULL root.
Node *bst_create(void) {
    return NULL;
}

// Usage: Destructor for a binary tree. Starting at the root, a postorder traversal will
//        be done to delete each node of the tree.
//
// Parameters: root of a binary search tree.
// Return value: void.
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}

// Usage: This function calculates the height (also known as depth) of a binary search tree.
//
// Parameters: root of a binary search tree.
// Return value: height of binary search tree.
uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

// Usage: This function calculates the size (also known as the number of nodes) of a binary
//        search tree.
//
// Parameters: root of a binary search tree.
// Return value: height of a binary search tree.
uint32_t bst_size(Node *root) {
    if (root) {
        return 1 + bst_size(root->left) + bst_size(root->right);
    }
    return 0;
}

// Design inspired by lecture 18 (Trees and BST's) by Prof. Long.
//
// Usage: This function searches a BST for a node containing oldspeak starting from the root
//        of the tree. If a node is found containing the specified oldspeak then the root of
//        tree is returned. Otherwise a NULL root is returned.
//
// Parameters: root of a BST and oldspeak.
// Return value: root of a BST or NULL root.
Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) == 0) { // found the node with oldspeak
            return root;
        }
        if (strcmp(root->oldspeak, oldspeak) > 0) { // if root->oldspeak > oldspeak
            branches += 1;
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) { // else if root->oldspeak < oldspeak
            branches += 1;
            return bst_find(root->right, oldspeak);
        }
    }
    return NULL; // no node with oldspeak was found
}

// Design inspired by lecture 18 (Trees and BST's) by Prof. Long.
//
// Usage: This function will insert a new node containing oldspeak and newspeak in the BST.
//        Duplicates of oldspeak will not be inserted, and if the tree does not exist prior
//        to insertion, a new BST is made.
//
// Parameters: root of a BST, oldspeak, and newspeak.
// Return value: the root of a BST after inserting the node, or a new BST.
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) == 0) { // found a duplicate oldspeak, dont insert
            return root;
        }
        if (strcmp(root->oldspeak, oldspeak) > 0) { // if root->oldspeak > oldspeak
            branches += 1;
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else { // else root->oldspeak < oldspeak
            branches += 1;
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    }
    // else you make a new root for the tree
    return node_create(oldspeak, newspeak);
}

// Usage: This function performs an inorder traversal of the BST to print the nodes
//        in a lexicographic order.
//
// Parameters: root of a BST.
// Return value: void.
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}
