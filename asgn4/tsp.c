// The design of this Test Harness has been inspired by Eugene's sections regarding file handling (reading/writing)
// and memory allocation of the cities array.
//
// The implementation of the DFS (Depth-first search) algorithm has been inspired and based off of the provided pseudocode
// by Prof. Long. as well as Eugene's explanations of the DFS algorithm.
#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "hvui:o:"

static bool undirect = false;
static bool verbose = false;
static bool first_path = true; // check if the first path has not been found in DFS
static bool header = false;
static uint32_t recursive = 0; // count recursive calls
static uint32_t vertices; // stores number of vertices specified in infile

// Usage: This function implements the DFS (Depth-first search) algorithm and starts from a start vertex (0) and marks that vertex as visited.
// Then the function will search for child vertex's with a connected edge to vertex (v), and if such an edge exists it will be pushed onto the path
// and then recursively call itself onto the connected vertex (w) just found.
//
// Once a suitable hamiltonian path (a path that visits each vertex exactly once) it will be kept track of to keep the shortest path out of all the
// paths found during the DFS functions runtime.
//
// Return value: void.
// Parameters: a graph, a start vertex, a current path, a shortest path, address of a cities array, and an outfile.
void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile) {
    graph_mark_visited(G, v);
    recursive += 1;

    // if the current path being made is longer than the current shortest, prematurely stop computing that path.
    if (path_length(curr) >= path_length(shortest) && path_length(shortest) != 0) {
        return;
    }

    // checks for valid hamiltonian path.
    if ((path_vertices(curr) == graph_vertices(G) - 1) && graph_has_edge(G, v, 0)) {
        path_push_vertex(curr, 0, G);

        // copy current path to shortest, if current has a smaller path length.
        if (path_length(curr) < path_length(shortest) || first_path) {
            path_copy(shortest, curr);
            first_path = false;
        }
        // if verbose stats is enabled, print the hamiltonian paths as they are found.
        if (verbose) {
            path_print(curr, outfile, cities);
        }
        uint32_t x;
        path_pop_vertex(curr, &x, G);
    }

    for (uint32_t w = 0; w < graph_vertices(G); w += 1) {
        if (graph_has_edge(G, v, w) && !graph_visited(G, w)) {
            graph_mark_visited(G, w); // mark child vertex as visited.
            path_push_vertex(curr, w, G); // push child vertex onto path.
            dfs(G, w, curr, shortest, cities, outfile); // recursively call dfs on child vertex.
            graph_mark_unvisited(G, w); // backtracing...
            path_pop_vertex(curr, &w, G);
        }
    }
}

int main(int argc, char **argv) {
    FILE *infile = stdin;
    FILE *outfile = stdout;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': {
            header = true;
            break;
        }
        case 'v': {
            verbose = true;
            break;
        }
        case 'u': {
            undirect = true;
            break;
        }
        case 'i': {
            infile = fopen(optarg, "r");
            break;
        }
        case 'o': {
            outfile = fopen(optarg, "w");
            break;
        }
        default: {
            header = true;
            break;
        }
        }
    }

    if (header) {
        printf("SYNOPSIS\n"
               "  Traveling Salesman Problem using DFS.\n"
               "\n"
               "USAGE\n"
               "  ./tsp [-u] [-v] [-h] [-i infile] [-o outfile]\n"
               "\n"
               "OPTIONS\n"
               "  -u             Use undirected graph.\n"
               "  -v             Enable verbose printing.\n"
               "  -h             Program usage and help.\n"
               "  -i infile      Input containing graph (default: stdin)\n"
               "  -o outfile     Output of computed path (default: stdout)\n");
        return 0;
    }

    // character buffer.
    char buff[1024];

    // if file fails to open print error and end program.
    if (infile == NULL) {
        fprintf(stderr, "Error: failed to open infile.\n");
        return 0;
    } else {
        // reads first line containing number of vertices for graph.
        fscanf(infile, "%" SCNu32 "\n", &vertices);
    }

    // if the graph file being read has more vertices than allowed, print error
    // message and end program.
    if (vertices > VERTICES || vertices <= 0) {
        fprintf(stderr, "Error: malformed number of vertices.\n");
        return 0;
    }

    // if the graph only has 1 vertices then theres nowhere to go, in other words
    // the only path that exists is from the origin to itself.
    if (vertices == 1) {
        fprintf(stderr, "There's nowhere to go.\n");
        return 0;
    }

    // array containing city names (array of strings).
    char **city_names = (char **) calloc(vertices, sizeof(char *));

    for (uint32_t x = 0; x < vertices; x += 1) {
        fgets(buff, 1024, infile);
        buff[strlen(buff) - 1] = '\0';
        city_names[x] = strdup(buff);
    }

    // make graph.
    Graph *G = graph_create(vertices, undirect);
    static uint32_t i, j, k;
    static int32_t m_edge = -1;
    while (!feof(infile)) {
        fscanf(infile, "%" SCNu32 " %" SCNu32 " %" SCNu32 "\n%n", &i, &j, &k, &m_edge);
        // if there are more city_names than vertices, print error, end program.
        if (m_edge == -1) {
            fprintf(stderr, "Error: malformed edge.\n");
            return 0;
        }
        graph_add_edge(G, i, j, k);
    }
    // close infile.
    fclose(infile);

    // initializing empty paths (of same capacity).
    Path *curr_p = path_create();
    Path *short_p = path_create();

    // begin DFS procedure.
    dfs(G, 0, curr_p, short_p, city_names, outfile);

    // print shortest path found in DFS.
    path_print(short_p, outfile, city_names);

    fprintf(outfile, "Total recursive calls: %" PRIu32 "\n", recursive);
    // close outfile.
    fclose(outfile);

    // deallocate memory for city_names array.
    for (uint32_t x = 0; x < vertices; x += 1) {
        free(city_names[x]);
    }
    free(city_names);

    // deallocate memory for graph, and current and shortest paths.
    graph_delete(&G);
    path_delete(&curr_p);
    path_delete(&short_p);
    return 0;
}
