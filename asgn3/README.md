# Assignment 3: Sorting - Putting your affairs in order
Serjo Barron

sepbarro

Fall 2021

## [files]
 - insert.c
 - insert.h
 - heap.c
 - heap.h
 - quick.c
 - quick.h
 - shell.c
 - shell.h
 - set.h
 - stats.c
 - stats.h
 - sorting.c
 - Makefile

## [description]
This program implements 4 sorting algorithms: Insertion, Shell, Heap, and Quick Sorts to order pseudo-randomly generated arrays of length n in increasing order starts from index 0 having the smallest element, then index (n-1) having the largest element in the given array. All the sorting algorithms have been implemented in functions and are linked to a test harness "sorting.c". The test harness will the main point of user-interaction to select and print the results of the chosen algorithm(s).

The test harness includes a synopsis page with the available options to select, and out of those options the user has the choice to define the seed the array is generated with, the length of the array to generate, and the amount of elements to print after the array has been sorted. If the user does not wish to define the seed, array length, or number of elements printed the program will use default values appropriate to the array being sorted.

If all or more than one sorting function is called, the array that the sorting functions will deal with are the same array. By the same array, that also means the array being sorted across the functions keeps the same length and seed.

## [instructions]
First ensure all the requires files listed above are present in the directory you are running the program in. Once they have been checked to be present, run the command "make clean" as a precaution to the object files being linked to the test harness are the most recent. Next run the command "make all" to successfully link all the header files and ".c" files including the implementation of the sorting algorithms to the test harness "sorting.c". Once they have all been linked you should be able to run the binary by typing "./sorting". You will be prompted by a synopsis message including the command-line-options and instructions on how to format them.

An example of running the program is typing "./sorting -i -n 15 -p 14 -r 2021" which will run the insertion sort function with an array length of 15, print 14 elements of the sorted array, and use the seed 2021 to pseudo-randomly generate the elements of the array to be sorted.
