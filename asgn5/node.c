#include "node.h"

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// Usage: This function will allocate memory for a node, and set that nodes symbol and frequency as specified by the given parameters.
//        The left and right child will initially be NULL upon creation.
//
// Parameters: the symbol and frequency for the node.
// Return value: a node.
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->symbol = symbol;
    n->frequency = frequency;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// Usage: This function will deallocate memory for a node and set its pointer to NULL.
//
// Parameters: the address of the node to be deleted.
// Return value: void.
void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

// Usage: This function will take 2 nodes, a right and left and combined their frequencies to be the frequency of a newly constructed node,
//        the parent node. The parent nodes right and left children will then be set to the given left and right node in the parameters.
//
// Parameters: a right and left node to be joined.
// Return value: the parent node.
Node *node_join(Node *left, Node *right) {
    uint64_t freq = left->frequency + right->frequency;
    Node *parent = node_create('$', freq);
    parent->left = left;
    parent->right = right;
    return parent;
}

// Usage: This function is for debugging, and will print information regarding the node such as its symbol, frequency, and its children.
//
// Parameters: a node.
// Return value: void.
void node_print(Node *n) {
    printf("[%" PRIu8 "] = %" PRIu64 ", left[%" PRIu8 "] = %" PRIu64 ", right[%" PRIu8
           "] = %" PRIu64 "\n",
        n->symbol, n->frequency, (n->left)->symbol, (n->left)->frequency, (n->right)->symbol,
        (n->right)->frequency);
    return;
}
