#!/bin/bash

OUT=$1
echo "x y z" > $OUT
i=0
while read l; do
	j=0
	for x in $l; do
		echo "$j $i $x" >> $OUT
		j=$(($j + 1))
	done
	echo >> $OUT
	i=$(($i + 1))	
done
