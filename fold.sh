#!/bin/bash

for file in `ls dataset/*.txt`
do
	l=`expr ${#file} - 12`
	f=${file:8:$l}
	for j in {0..0}
	do
		for i in {0..9}
		do
			echo -en "$1\t$f\t$i\t"
			./$1 $file $i
		done
	done
done
