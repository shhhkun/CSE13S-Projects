#include "graph.h"

#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// struct for graph
struct Graph {
    uint32_t vertices; // Number of vertices
    bool undirected; // bool for undirected graph
    bool visited[VERTICES]; // visited places
    uint32_t matrix[VERTICES][VERTICES]; // adjacency matrix
};

// Credit to Prof. Long
//
// Usage: This function creates a graph (an n by n matrix) and initializes all those elements to 0
// to mark them as having no edge.
//
// Return value: a graph.
// Parameters: number of specified vertices, and whether the graph is undirected.
Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

// Credit to Prof. Long
//
// Usage: This function deletes a graph by freeing all the memory allocated to it, and then
// sets the pointer to the graph as NULL to prevent potential issues.
//
// Return value: void.
// Parameters: the address of the graph to delete.
void graph_delete(Graph **G) {
    free(*G); // free allocated memory of Graph via pointer to pointer
    *G = NULL;
    return;
}

// Usage: This function returns the amount of vertices (n) of the n by n matrix, also called
// the graph.
//
// Return value: number of vertices.
// Parameters: a graph.
uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

// Usage: This function checks whether i and j are within bounds of the matrix then will add a weight of k.
//
// Return value: boolean indicating success or failure.
// Parameters: a graph, i, j, and a weight k.
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if (i >= 0 && i < G->vertices && j >= 0 && j < G->vertices) {
        G->matrix[i][j] = k;
        if (G->undirected) {
            G->matrix[j][i] = k;
        }
        return true;
    }
    return false;
}

// Usage: This function checks whether an edge exists in the graph.
//
// Return value: a boolean indicating whether there was an edge found.
// Parameters: a graph, i, and j.
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if (i >= 0 && i < G->vertices && j >= 0 && j < G->vertices && G->matrix[i][j] > 0) {
        return true;
    }
    return false;
}

// Usage: This function returns an edge weight (k) with the specified i,j coordinates.
//
// Return value: an edge weight, or return 0.
// Parameters: a graph, i, and j.
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (graph_has_edge(G, i, j)) {
        return G->matrix[i][j];
    }
    return 0;
}

// Usage: This function checks whether a certain vertex has been visited.
//
// Return value: a boolean indicating whether it has been visited.
// Parameters: a graph, and the vertex being checked.
bool graph_visited(Graph *G, uint32_t v) {
    if (G->visited[v] == true) {
        return true;
    }
    return false;
}

// Usage: This function marks a vertex (v) as visited.
//
// Return value: void.
// Parameters: a graph, and a vertex.
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v >= 0 && v < G->vertices) {
        G->visited[v] = true;
        return;
    }
}

// Usage: This function marks a vertex (v) as unvisited.
//
// Return value: void.
// Parameters: a graph, and a vertex.
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v >= 0 && v < G->vertices) {
        G->visited[v] = false;
        return;
    }
}

// Usage: This function is meant for debugging purposes. The function itself
// prints the n by n matrix as a visual representation of the graph.
//
// Return value: void.
// Parameters: a graph.
void graph_print(Graph *G) {
    for (uint32_t x = 0; x < G->vertices; x += 1) {
        for (uint32_t y = 0; y < G->vertices; y += 1) {
            printf("%2" PRIu32 " ", G->matrix[x][y]);
            if (y == (G->vertices) - 1) {
                printf("\n");
            }
        }
    }
    return;
}
