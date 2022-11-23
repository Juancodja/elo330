#! /bin/bash


t=$1 # en segundos


./cfifo "fifo" $t > datafifo.dat &
./pfifo "fifo" $t 

./cshmem "/shared_mem" $t > datashmem.dat &
./pshmem "/shared_mem" $t


gnuplot <<'EOF'
set grid
set title "Tasa de transferencia"
set xlabel "tiempo ms"
set ylabel "bits"
plot 'datafifo.dat' w lp lw 2 title 'Shmem', 'datashmem.dat' w lp lw 2 title 'Fifo'

set terminal svg
set output "img.svg"
replot

set terminal pngcairo
set output "img.png"
replot
EOF