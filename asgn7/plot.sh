#!/usr/bin/env bash

./graphs -t 100000 -x 1000 -1 | awk -F, '{
    split($1, x, " ");
    split($2, y, " ");
    print x[1], y[1] > "/tmp/points.dat"
    print x[1], y[1]
}'

echo -n "Plotting... "
gnuplot <<EOF
set terminal pdf
set key outside
set zeroaxis

set log x

set xlabel "HashTable size"
set ylabel "Average BST size"

set output "htsize_abs.pdf"
set title "Increasing HashTable size with Average BST size"

plot "/tmp/points.dat" with lines title "ht\\\_size"
EOF
echo "done."
