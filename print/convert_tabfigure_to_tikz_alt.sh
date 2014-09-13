#!/bin/bash

OUT=$1
read N
echo -n "x y " > $OUT
for i in `seq 0 $(( $N - 1 ))`; do
	echo -n "b$i " >> $OUT;
done
echo >> $OUT;
i=0
while read l; do
	j=0
	X=$(echo -e "$l" | tr ' ' '@' | tr '\t' ' ');
	for x in $X; do
		echo "$j $i $x"
		j=$(($j + 1))
	done
	echo
	i=$(($i + 1))
done | tr '@' ' ' >> $OUT
