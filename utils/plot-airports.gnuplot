set datafile separator comma
set key autotitle columnheader
set xrange [-180:180]
set yrange [-90:90]
set grid
set size ratio -1
set xlabel "Latitude"
set ylabel "Longitude"

set term pdfcairo font "Nexus,12" size 20cm,10cm
set output 'airports.pdf'

plot '../data/ground-stations/airports.csv' using 3:2
