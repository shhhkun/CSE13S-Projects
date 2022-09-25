#include "node.h"
#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

// Usage: This function is the constructor of the stack, and will set its capacity as specified.
//
// Parameters: the capacity of the stack.
// Return value: a stack.
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

// Usage: This function is the destructor of the stack, and will deallocate memory of the stacks contents,
//        with the exception of touching the memory of the nodes.
//
// Parameters: the address of the stack to be deleted.
// Return value: void.
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

// Usage: This function will check whether the stack is empty.
//
// Parameters: a stack.
// Return value: a boolean indicating whether the stack is empty.
bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

// Usage: This function will check whether the stack is full.
//
// Parameters: a stack.
// Return value: a boolean indicating whether the stack is full.
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

// Usage: This function will return the current size of the stack.
//
// Parameters: a stack.
// Return value: the current size of the stack.
uint32_t stack_size(Stack *s) {
    return s->top;
}

// Usage: This function will push a node onto the stack.
//
// Parameters: a stack and the node to be pushed.
// Return value: a boolean value indicating success or failure.
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = n;
    s->top += 1;
    return true;
}

// Usage: This function will pop a node off the stack, and pass the node popped to the Node 'n'
//        specified in the parameters.
//
// Parameters: a stack and an address of a node whether the popped node will be stored.
// Return value: a boolean value indicating success or failure.
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    s->top -= 1;
    *n = s->items[s->top];
    return true;
}

// Usage: This function is for debugging purposes and will simply print the contents (nodes) of the stack.
//
// Parameters: a stack.
// Return value: void.
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->capacity; i += 1) {
        node_print(s->items[i]);
        printf(", ");
    }
    printf("\n");
    return;
}
