#include "node.h"
#include "code.h"
#include "pq.h"
#include "io.h"
#include "defines.h"
#include "stack.h"

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

static Code c;
bool x = true; // used to initialize the code only once.

static uint32_t symbols = 0; // count symbols in build_tree, used later in dump_tree.

static uint8_t dump_buf[BLOCK];
static uint32_t dump_bytes = 0;

// Usage: This function will create a Huffman tree by using a histogram table. Using that histogram table, a priority queue will be populated with
//        nodes, and then be dequeued (twice) to create a parent node. THe parent node will then be enqueued back onto the queue, and this process
//        will be repeated until the size of the queue is 1, meaning there is one node remaining. The remaining node is the root of the Huffman tree
//        and will be used in later functions to traverse the tree.
//
// Parameters: a histogram containing the frequencies of symbols.
// Return value: the root node of the Huffman tree.
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq = pq_create(ALPHABET);
    for (uint32_t i = 0; i < ALPHABET; i += 1) { // populate queue with nodes.
        if (hist[i] > 0) {
            enqueue(pq, node_create(i, hist[i]));
            symbols += 1;
        }
    }
    Node *left;
    Node *right;
    Node *root;
    while (pq_size(pq) > 1) { // dequeue while not on root node.
        dequeue(pq, &left);
        dequeue(pq, &right);

        Node *parent = node_join(left, right);
        enqueue(pq, parent);
    }
    dequeue(pq, &root);
    pq_delete(&pq);
    return root;
}

// Usage: This function will create corresponding codes for each symbol (leaf node) of the Huffman tree. This will be done by traversing the tree
//        and pushing a 1 or 0 onto the code based on whether the traversal path is left or right. If a leaf node is reached, then the symbol of that
//        node will be assigned the current calculated code, then we will traverse back up the tree to assign codes for every leaf node.
//
// Parameters: the root of a Huffman tree and a code table.
// Return value: void.
void build_codes(Node *root, Code table[static ALPHABET]) {
    if (x) { // initialize code once.
        c = code_init();
        x = false;
    }
    if (root != NULL) {
        if (root->left == NULL && root->right == NULL) {
            table[root->symbol] = c;
        } else {
            uint8_t bit;
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, &bit);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &bit);
        }
    }
    return;
}

// Usage: This function will perform a post-order traversal of the Huffman tree to create a "tree dump", which is a series of L's, I's, and symbol's
//        which will be written to outfile. This is done by traversing until a leaf node is encountered, to which then we buffer an L and that nodes
//        corresponding symbol to the buffer. Otherwise, an interior node was encountered to which an I is buffered.
//
// Parameters: outfile and the root of the Huffman tree.
// Return value: void.
void dump_tree(int outfile, Node *root) {
    if (root != NULL) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        if (root->left == NULL && root->right == NULL) {
            dump_buf[dump_bytes] = 'L';
            dump_bytes += 1;
            dump_buf[dump_bytes] = root->symbol;
            dump_bytes += 1;
        } else {
            dump_buf[dump_bytes] = 'I';
            dump_bytes += 1;
        }
    }
    if (dump_bytes == (3 * symbols) - 1) {
        write_bytes(outfile, dump_buf, dump_bytes);
    }
}

// Usage: This function will read the "tree dump" written to outfile by the dump_tree() function above, and will reconstruct the Huffman tree
//        by iterating over each byte of the dump. If an L is encountered in the tree dump, we skip to the next index of the dump array and
//        create a node for that symbol to push onto the stack. Otherwise, an interior node was encountered to which we pop the stack twice
//        and create a parent node to be pushed onto the stack. This process will be done for the length of the dump, and the remaining node
//        on the stack is the root of the reconstructed Huffman tree.
//
// Parameters: the length of the tree dump, and a tree dump array.
// Return value: the root of the reconstructed Huffman tree.
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(ALPHABET);
    Node *left;
    Node *right;
    Node *root;
    for (uint16_t i = 0; i < nbytes; i += 1) {
        if (tree[i] == 'L') { // if on a leaf node 'L'
            i += 1;
            Node *leaf = node_create(tree[i], 0); // create node of the symbol after 'L'
            stack_push(s, leaf);
        } else { // else it is a interior node 'I'
            stack_pop(s, &right);
            stack_pop(s, &left);
            Node *parent = node_join(left, right);
            stack_push(s, parent);
        }
    }
    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}

// Usage: This function will perform a post-order traversal of the Huffman tree and deallocate (delete the nodes) all the memory that has been
//        allocated by the nodes of the tree.
//
// Parameters: the address of the root of the huffman tree.
// Return value: void.
void delete_tree(Node **root) {
    if (*root != NULL) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        node_delete(root);
    }
}
