#pragma once

#include "node.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct PriorityQueue PriorityQueue;

struct PriorityQueue {
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
    Node **nodes;
};

PriorityQueue *pq_create(uint32_t capacity);

void pq_delete(PriorityQueue **q);

bool pq_empty(PriorityQueue *q);

bool pq_full(PriorityQueue *q);

uint32_t pq_size(PriorityQueue *q);

bool enqueue(PriorityQueue *q, Node *n);

bool dequeue(PriorityQueue *q, Node **n);

void pq_print(PriorityQueue *q);
