set datafile separator ",:" 
set key autotitle columnheader
set terminal gif animate size 2000,2000
set output 'output.gif'

unset xtics
unset ytics
unset ztics
unset border
set xrange [-8e6:8e6]
set yrange [-8e6:8e6]
set zrange [-8e6:8e6]
set parametric
set isosamples 100,100 
unset key
set hidden3d

# number of nodes per time slot
EARTH=6.370e6
sats=ARG1
ground=ARG2
numsats=ARG3
numsamples=ARG4

do for [j=0:numsamples] {
	set title 'time '.j
	splot [-pi:pi][-pi/2:pi/2] EARTH*cos(u)*cos(v), EARTH*sin(u)*cos(v), EARTH*sin(v), \
	      ground using 1:2:3 lt rgb "green", \
	      sats using 3:4:5:2 every ::(j*numsats)::((j+1)*numsats) lt rgb "blue"
	      
}
