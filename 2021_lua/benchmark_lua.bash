#!/bin/bash

 for ele in {01..06} ; do
	echo "day${ele}"
	for part in {1,2} ; do
		time lua5.4 aoc.lua $ele $part
	done
	echo
done
