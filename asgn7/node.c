#include "node.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Usage: The constructor of a node. A node will contain oldspeak and newspeak if and only
//        if the specified oldspeak or newspeak is not NULL. The left and right children of
//        the node will also initially be set to NULL.
//
// Parameters: oldspeak and newspeak.
// Return value: a node.
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->oldspeak = NULL;
    n->newspeak = NULL;
    if (oldspeak != NULL) {
        n->oldspeak = strdup(oldspeak);
    }
    if (newspeak != NULL) {
        n->newspeak = strdup(newspeak);
    }
    n->left = NULL;
    n->right = NULL;
    return n;
}

// Usage: Destructor for a node. Allocated memory for oldspeak and newspeak will only be
//        free'd if it exists (not NULL).
//
// Parameters: a node.
// Return value: void.
void node_delete(Node **n) {
    if (*n) {
        if ((*n)->oldspeak) {
            free((*n)->oldspeak);
        }
        if ((*n)->newspeak) {
            free((*n)->newspeak);
        }
        free(*n);
        *n = NULL;
    }
}

// Usage: This function prints the contents of a node depending on whether it contains
//        both a oldspeak and newspeak, or if it only contains an oldspeak.
//
// Parameters: a node.
// Return value: void.
void node_print(Node *n) {
    if (n->oldspeak && n->newspeak) { // oldspeak and newspeak both exist
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
    if (n->oldspeak && !n->newspeak) { // only oldspeak exists
        printf("%s\n", n->oldspeak);
    }
    return;
}
