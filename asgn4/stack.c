#include "stack.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top; // index of the empty element (top of stack)
    uint32_t capacity; // maximum number of items that can be pushed
    uint32_t *items; // array of items
};

// Credit to Prof. Long
//
// Usage: This function serves as a constructor of making a stack. It will allocate memory for the stack
// struct and, allocate the specified capacity of items to be held in the stack.
//
// Return value: a stack.
// Parameters: specified capacity for the stack.
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

// Credit to Prof. Long
//
// Usage: This function deallocates memory for the stack, and sets the pointer to the stack as NULL
// to prevent potential errors.
//
// Return value: void.
// Parameters: the address of a stack.
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

// Usage: This function returns the current stack size (the number of items inside the stack).
//
// Return value: number of items in the stack.
// Parameters: a stack.
uint32_t stack_size(Stack *s) {
    return (s->top);
}

// Usage: This function checks whether the stack is empty.
//
// Return value: a boolean indicating whether the stack is empty.
// Parameters: a stack.
bool stack_empty(Stack *s) {
    if ((s->top) == 0) {
        return true;
    } else {
        return false;
    }
}

// Usage: This function checks whether the stack is full.
//
// Return value: a boolean indicating whether the stack is full.
// Parameters: a stack.
bool stack_full(Stack *s) {
    if ((s->top) == (s->capacity)) {
        return true;
    } else {
        return false;
    }
}

// Usage: This function pushes an item onto the stack.
//
// Return value: a boolean indicating whether the item was pushed.
// Parameters: a stack, and the value to push.
bool stack_push(Stack *s, uint32_t x) {
    if (stack_full(s)) {
        return false;
    } else {
        s->items[s->top] = x;
        s->top += 1;
        return true;
    }
}

// Usage: This function pops an item off the stack.
//
// Return value: a boolean indicating whether the item was popped.
// Parameters: a stack, and an address of a variable where the popped item will be stored.
bool stack_pop(Stack *s, uint32_t *x) {
    if (!stack_empty(s)) {
        s->top -= 1;
        *x = s->items[s->top]; // dereference x to store popped element in address of x.
        return true;
    } else {
        return false;
    }
}

// Usage: This functions peeks what value is currently at the top of the stack.
//
// Return value: a boolean indicating whether an item was peeked.
// Parameters: a stack, and an address of a variable where the peeked item will be stored.
bool stack_peek(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        return false;
    } else {
        *x = s->items[(s->top) - 1]; // dereference x to store peeked element in address of x.
        return true;
    }
}

// Usage: This functions takes two stacks, one will be the source, and the other the destination.
// The contents of the source stack, including the state of its top will be copied over to the destination
// stack.
//
// Return value: void.
// Parameters: a source and destination stack.
void stack_copy(Stack *dst, Stack *src) {
    for (uint32_t x = 0; x < src->capacity; x += 1) {
        dst->items[x] = src->items[x];
    }
    dst->top = src->top;
    return;
}

// Credit to Prof. Long
//
// Usage: This function will serve as a helper function (to path_print() in "path.c")
// to print the current path of cities into an outfile.
//
// Return value: void.
// Parameters: a stack, an outfile, and a cities array.
void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
    return;
}
