#!/bin/bash

for ele in *testing*
do
	mv "$ele" ".$ele"
done
