#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t edge; // calculated edge weight for path_pop/push functions.

struct Path {
    Stack *vertices; // vertices of path
    uint32_t length; // length (summation) of path
};

// Usage: This function allocates memory for the path struct, and creates a stack for vertices.
//
// Return value: a path.
// Parameters: void.
Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    if (p) {
        p->vertices = stack_create(VERTICES);
        p->length = 0;
        if (!p->vertices) {
            free(p);
            p = NULL;
        }
    }
    return p;
}

// Usage: This function deallocates memory for the path, and deletes the contents of stack vertices.
// It sets the pointer to the path as NULL to prevent potential errors.
//
// Return value: void.
// Parameters: the address of a path.
void path_delete(Path **p) {
    if (*p && (*p)->vertices) {
        stack_delete(&((*p)->vertices));
        free((*p)->vertices);
        free(*p);
        *p = NULL;
    }
    return;
}

// Usage: This function pushes a vertex onto the path stack (vertices) and adds the corresponding edge weight
// onto the length.
//
// Return value: a boolean indicating whether the vertex was pushed onto the path.
// Parameters: a path, a vertex, and a graph.
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    if (stack_full(p->vertices)) {
        return false;
    } else {
        if (stack_empty(p->vertices)) { // checks if the path is at origin (0).
            edge = graph_edge_weight(G, 0, v);
        } else { // otherwise the stack isn't empty.
            uint32_t x;
            stack_peek(p->vertices, &x);
            edge = graph_edge_weight(G, x, v);
        }
        stack_push(p->vertices, v);
        p->length += edge;
        return true;
    }
}

// Usage: This function pops a vertex off the path stack (vertices) and subtracts the corresponding edge weight
// off of the length.
//
// Return value: a boolean indicating whether the vertex was popped off the path.
// Parameters: a path, an address of a variable where the vertex (v) will be stored, and a graph.
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    if (stack_empty(p->vertices)) {
        return false;
    } else {
        uint32_t x; // peeked vertex.
        uint32_t y; // popped vertex.
        stack_pop(p->vertices, &y);
        if (path_vertices(p) == 0) { // if the path is empty, the vertex before v is 0.
            edge = graph_edge_weight(G, 0, y);
        } else { // otherwise the stack is not on the last element.
            stack_peek(p->vertices, &x);
            edge = graph_edge_weight(G, x, y);
        }
        p->length -= edge;
        *v = y;
        return true;
    }
}

// Usage: This function returns the current amount of vertices inside the path (the size of the vertices stack).
//
// Return value: the number of vertices in the path.
// Parameters: a path.
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

// Usage: This function returns the current path length of a path.
//
// Return value: the path length.
// Parameters: a path.
uint32_t path_length(Path *p) {
    return p->length;
}

// Usage: This function will copy the contents of the path (the vertices stack) from a source path to a destination path.
// The length of the source path will also be copied over to the destination path.
//
// Return value: void.
// Parameters: a source and destination path.
void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
    return;
}

// Usage: This function will print the current path based on the contents of the vertices stack and the path length into
// an outfile.
//
// Return value: void.
// Parameters: a path, an outfile, and the address of a cities array.
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %" PRIu32 "\n", p->length);
    fprintf(outfile, "Path: %s -> ", cities[0]); // used to print origin/start of a path
    stack_print(p->vertices, outfile, cities);
    return;
}
