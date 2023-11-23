#!/bin/sh

set -u
set -e

search=$1

# Get the ids of sats matching the search term
if [ ! -f ids.txt ]
then
	curl https://celestrak.com/pub/satcat.txt | grep "$search" | cut -b14-18 > ids.txt
fi

# Download TLE data for each sat
while read id
do
	if [ ! -f $id.txt ]
	then
		wget "https://celestrak.com/satcat/tle.php?CATNR=$id" -O $id.txt
	fi
	# 6000 s ~ 95 min ~ 1 orbital period
done < ids.txt

while read id
do
	if [ ! -f $id.waypoints ]
	then
		./tle2wps.py $id.txt 2020 08 28 6000 > $id.waypoints
	fi
done < ids.txt
