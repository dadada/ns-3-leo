set datafile separator comma
set key autotitle columnheader
set terminal gif animate
set output 'output.gif'

unset xtics
unset ytics
unset ztics
unset border
unset key
set view equal xyz

# number of nodes per time slot
n=1200

do for [j=0:100] {
	set title 'time '.j
	splot 'leo-circular-orbit-tracing-example.csv' using 3:4:5:2 every ::(j*n)::((j+1)*n)
}
