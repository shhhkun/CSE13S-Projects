# Assignment 2: A Little Slice of Pi
Serjo Barron

sepbarro

Fall 2021

## [files]
 - e.c
 - newton.c
 - euler.c
 - bbp.c
 - viete.c
 - madhava.c
 - mathlib-test.c
 - mathlib.h
 - Makefile

## [description]
This program imiates a few selected functions and mathematical constants from the <math.h> library. The specific mathematical constants computed are e and pi, as will as a newton square root function that mimicks sqrt() in <math.h>.
All of the impltemented functions listed as ".c" files in the list of files above will all be linked to one test harness, mathlib-test.c. The test harness will be the main point of user-interaction as it prints the result, differences between the <math.h> library, and statistics of the implemented functions. 

Depending on what option the user selects from the synopsis page of the test harness, the test harness will follow up by printing a series of messages corresponding to a chosen function, or all if it is selected out of the option menu.

## [instructions]
First, ensure that all the required files (listed above) for the program to run are present in the correct directory. Once all the required files are present, run the command make clean as a precaution to remove the mathlib-test binary, and ensure the one being run is up to date. Next, run the command make format to ensure that all the files in the directory are clang-formatted. Lastly, you can use the command make or make all which will link all the functions defined in e.c, newton.c, euler.c, viete.c, and madhava.c to mathlib-test.c and generate an executable.

After you have generated the binary "mathlib-test", you should then be able to run the command ./mathlib-test which will direct you to a synopsis page including the instructions to print mesages and list of options of the various messages to print. From there, you can run commands such as ./mathlib-test -e to print the corresponding message of the e.c calculations, or ./mathlib-test -es if you wish to also see the verbose statistics (number of terms) that it took to compute the value of e.

Once the user has run the command the program will automatically print all the corresponding messages to the tests being run and terminate right after. If the user wishes to view more of the options listed in the synopsis page, they are required to once again run the command ./mathlib-test -option(s). 
