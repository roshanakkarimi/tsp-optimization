#!/bin/bash

gcc -ansi -pedantic -Wall -Werror -c /Users/roshanakkarimi/Documents/Projects/OR2/TSP-OR/src/utilities.c -std=c99
gcc -ansi -pedantic -Wall -Werror -c /Users/roshanakkarimi/Documents/Projects/OR2/TSP-OR/src/main.c -std=c99
gcc -o main.exe main.o utilities.o
./main.exe -f ../data/att48.tsp -model_type 1
