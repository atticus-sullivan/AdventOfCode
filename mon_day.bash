#!/bin/bash

if [[ "$1" != [0-9][0-9] ]] ; then
	echo "First parameter must be the number of the day"
	return -1
fi

eval $(luarocks path)

luamon -t ../aoc.lua -- "$1"
