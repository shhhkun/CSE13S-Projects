# Assignment #4: The Perambulations of Denver Long
Serjo Barron

sepbarro

Fall 2021

## [files]
 - graph.h
 - graph.c
 - path.h
 - path.c
 - stack.h
 - stack.c
 - tsp.c
 - vertices.h
 - Makefile

## [description]
This program implements 3 ADTS (abstract data types) which are all used inside a main Test Harness called "tsp.", tsp being an acronym for Traveling Sales Person. The first is the graph ADT, which serves to create a n by n matrix, where the maximum value of n is 26, which we call as our graph. The graph ADT also includes various functions for adding edges which include weight, which will be our method of showing distance between vertexes. The second is a stack ADT, which implements a stack (using an array), which will be our way of storing data. And since it is a stack, there will be functions to push, pop, and peek the items inside the stack which will be useful for the last ADT. The third and last ADT is the path ADT, which will making use of the stack ADT implemented as a means of storing data. The path ADT however includes functions to formulate paths which is the purpose of this program, and manipulate them by popping/pushing vertexes and keeping track of path lengths.

It should be noted though that a common theme with all the 3 ADT's implemented for the program to work is a constructor and destructor. The constructor will allocate sufficient memory for the program, and the destructor will deallocate said memory to ensure the program runs without memory leaks.

Once put together, these 3 ADT's will be used in the actual purpose of this assignment, using the DFS (depth-first search) algorithm to search Hamiltonian paths. What is a Hamiltonian path? In brief a Hamiltonian path is a path which visits each vertex exactly once, and makes a path starting from a origin/home, and ending on that origin/home. An example of this could be a path from 0 to 1, 1 to 2, and 2 to 0, which explicitly has a path starting from the origin (0), and ending at that origin (0) while also having visited each vertex exactly once (with an exception of the origin vertex).

The Test Harness as said will be the main point of user-interaction, and as such (tsp.c) will be including the code required to handle file properly, meaning that it will read the infile and write the results of the DFS to an outfile. This also means that the graph files being read must be in a specified format, which in brief is the vertices as the first line, the city names in the next few, and then the (i,j,k) coordinates which correspond to the edge weight to be added to the graph. Once run, the program will find the shortest path and give the results which are the shortest path found and the amount of recursive calls to the DFS function made to search said path.

## [instructions]
Once all required files (as listed above) are present in the right directory, you have the means to properly run DFS on a graph. This means however you must also have a proper graph with certain syntax for it to be properly read. An example of how the graph files should be formatted is below:

3\
start\
middle\
end\
0 1 2\
1 2 5\
2 0 6

As seen, the amount of vertices is specified in the first line, and the amount of vertices also determines how many "cities" should exist, which in this case is 3. The lines after the city names are the (i,j,k) coordinates with (i,j) representing the place the weight will be added on the n by n matrix (graph), and the last number on that line (k) specified the weight of that vertex. Once the graph desired to use the DFS algorithm on has been properly formatted, you can now run the program. First run "./tsp -h" which will prompt you with a synopsis page of the available options when running DFS on a graph.

Out of those options, the main important ones is whether the user wants to specify an infile and outfile, if they want to enable verbose stats (meaning that the found Hamiltonian paths will be printed as they are found), and if they want to toggle a graph as undirected. Undirected meaning that an edge exists both from (i,j) and (j,i), as opposed to the default directed path of (i,j). Once you have been aquainted with the proper syntax of how to run the tsp.c file on a graph, you can then open the specified outfile to see the results of the graph, which will contain the shortest path found and the amount of recursive calls on DFS.

An example of running the program goes as follows, "./tsp -i home.graph -o results", which means that tsp will run DFS on home.graph which for the sake of clarity will be the graph previously made above, and that the shortest path found and the amount of recursive calls will be inside the file "results". Once DFS has finished finding the paths the program will terminate, and if the user wishes to test another graph then follow the same procedure as before.
