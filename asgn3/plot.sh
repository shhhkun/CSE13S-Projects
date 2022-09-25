#!/usr/bin/env bash

make clean && make

./sortgraph -a -n 100000 | awk -F, '{
    split($1, n, " ");
    split($2, heap, " ");
    split($3, shell, " ");
    split($4, insert, " ");
    split($5, quick, " ");
    print n[1], heap[1] > "/tmp/heap.dat"
    print n[1], shell[1] > "/tmp/shell.dat"
    print n[1], insert[1] > "/tmp/insert.dat"
    print n[1], quick[1] > "/tmp/quick.dat"
}'

echo -n "Plotting... "
gnuplot <<EOF
set terminal pdf
set key outside
set zeroaxis

set xrange [ 1000 : 100000 ]
set yrange [ 1000 : 10000000000 ]

set log x
set log y

set xlabel "elements"
set ylabel "moves"

set output "moveshigh.pdf"
set title "Number of elements vs moves taken"
plot "/tmp/heap.dat" with lines title "Heap Sort", \
     "/tmp/shell.dat" with lines title "Shell Sort", \
     "/tmp/insert.dat" with lines title "Insertion Sort", \
     "/tmp/quick.dat" with lines title "Quick Sort"
EOF
echo "done."
