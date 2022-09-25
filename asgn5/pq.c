#include "node.h"
#include "pq.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

// Min-Heap functions & helper-functions:

// Usage: This function simply swaps the values of x and y.
//
// Parameters: a node (x), and a node (y).
// Return value: void.
void swap(Node *x, Node *y) {
    Node t = *x;
    *x = *y;
    *y = t;
    return;
}

// Usage: This function finds the min-child (node with the least frequency) between first and last index.
//
// Parameters: a node, first index, last index. (1 based indexing)
// Return value: index of min-child.
uint32_t min_child(Node **nodes, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && (nodes[right - 1])->frequency < (nodes[left - 1])->frequency) {
        return right;
    }
    return left;
}

// Usage: This function performs a swap to fix a heap as specified by the first and last index, which point
//        to which part of the heap tree to traverse over and apply a swap.
//
// Parameters: a node, first index, last index. (1 based indexing)
// Return value: void.
void fix_heap(Node **nodes, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t least = min_child(nodes, mother, last);
    while (mother <= last / 2 && !found) {
        if ((nodes[mother - 1])->frequency > (nodes[least - 1])->frequency) {
            swap(nodes[mother - 1], nodes[least - 1]);
            mother = least;
            least = min_child(nodes, mother, last);
        } else {
            found = true;
        }
    }
    return;
}

// Priority Queue functions:

// Usage: This function is a constructor for the priority queue of capacity size.
//
// Parameters: capacity.
// Return value: the created priority queue.
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->head = 0;
        q->tail = 0;
        q->capacity = capacity;
        q->nodes = (Node **) calloc(capacity, sizeof(Node *));
        if (!q->nodes) {
            free(q);
            q = NULL;
        }
    }
    return q;
}

// Usage: This function is a destructor for our priority queue.
//
// Parameters: the address of a priority queue. (as specified by a double-pointer)
// Return value: void.
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->nodes) {
        free((*q)->nodes);
        free(*q);
        *q = NULL;
    }
    return;
}

// Usage: This function checks whether the priority queue is empty.
//
// Parameters: a priority queue.
// Return value: a boolean indicating if the queue is empty.
bool pq_empty(PriorityQueue *q) {
    if (q->tail == 0) {
        return true;
    }
    return false;
}

// Usage: This function checks whether the priority queue is full.
//
// Parameters: a priority queue.
// Return value: a boolean indicating whether the queue is full.
bool pq_full(PriorityQueue *q) {
    if (q->tail == q->capacity) {
        return true;
    }
    return false;
}

// Usage: This function returns the current size of the priority queue.
//
// Parameters: a priority queue.
// Return value: the size of the queue.
uint32_t pq_size(PriorityQueue *q) {
    return q->tail;
}

// Usage: This function will "enqueue" a node onto the priority queue. After the node is added,
//        this function will perform the necessary amount of heap_fix()'s to ensure the node with
//        the least frequency is of index 0 in the queue.
//
// Parameters: a priority queue and a node.
// Return value: a boolean indicating whether the node was enqueued.
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    q->nodes[q->tail] = n;
    q->tail += 1;
    uint32_t k = q->tail;
    while (k > 1) {
        uint32_t parent = k / 2;
        fix_heap(q->nodes, parent, q->tail);
        k = parent;
    }
    return true;
}

// Usage: This function will "dequeue" a node off the priority queue. After the tail of the queue has
//        been decremented, the head of the queue (index 0) will be swapped with whatever element is
//        at the end of the queue. The queue will then be fixed using fix_heap() to make sure the node
//        with the least frequency is once again the head of the queue.
//
// Parameters: a priority queue and a node to store the value of the node dequeued.
// Return value: a boolean indicating whether the node was dequeued.
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    q->tail -= 1;
    swap(q->nodes[q->head], q->nodes[q->tail]);
    *n = q->nodes[q->tail]; // the head of the queue is now the tail
    uint32_t k = q->tail;
    while (k > 1) {
        uint32_t parent = k / 2;
        fix_heap(q->nodes, parent, q->tail);
        k = parent;
    }
    return true;
}

// Usage: This function is for debugging purposes, and will simply print the symbol (integer value) of
//        the node alongside its frequency, for the length of the queue.
//
// Parameters: a priority queue.
// Return value: void.
void pq_print(PriorityQueue *q) {
    for (uint32_t i = 0; i < q->tail; i += 1) {
        printf("[%" PRIu8 "]: %" PRIu64 ", ", (q->nodes[i])->symbol, (q->nodes[i])->frequency);
    }
    printf("\n");
    return;
}
