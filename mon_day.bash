#!/bin/bash

day="$1"

if [[ "$day" == "" ]] ; then
	day="$(date +%d)"
fi

if [[ "$day" != [0-9][0-9] ]] ; then
	echo "First parameter must be the number of the day"
	exit -1
fi

eval $(luarocks path)

luamon -t ../aoc.lua -- "$day"
