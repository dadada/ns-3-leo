set datafile separator comma
set key autotitle columnheader
set terminal gif animate
set output 'output.gif'

unset xtics
unset ytics
unset ztics
unset border
set hidden3d
unset key
set view equal xyz

n=600

do for [j=0:100] {
	set title 'time '.j
	splot 'somefile' using 3:4:5:2 every ::(j*n)::((j+1)*n)
}
