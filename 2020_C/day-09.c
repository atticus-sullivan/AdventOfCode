#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "./read_file.h"

/*
Advent Of Code 2020
Copyright (C) 2024  Lukas Heindl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

long part1(struct long_content* input, int head_length, long* miss){
	// allocate memory to store the currently available numbers
	long* prev = malloc(sizeof(long)*head_length);
	int pointer = 0; // pointer that points to the next element that is to be removed

	// read header
	for(int i=0; i < head_length; i++){
		prev[i] = input->content[i];
	}

	// go thtough lines and search for two summands
	for(int i=head_length; i < input->length; i++){

		int verif = 0; // store if number could be verified
		// go through known numbers and search for a pair of numbers that sum up to input->content[i]
		for(int l = 0; l < head_length && !verif; l++){
			for(int k = 0; k < head_length && !verif; k++){
				// ignore number if same index is used
				if (k == l) continue;

				if(prev[l] + prev[k] == input->content[i]){
					// rewrite oldest number with the new verified one
					verif = 1;
					prev[pointer] = input->content[i];
					pointer = (pointer + 1) % head_length; // update pointer
				}
			}
		} // end for l

		if(verif) continue;
		// this is only executed if the numer couldn't be verified
		if(miss != NULL) *miss = input->content[i]; // if wanted store the number that couldn't be verified
		free(prev);
		return input->content[i];
	}
	free(prev);
	*miss = -1;
	return 0;
}

long part2(struct long_content* input, int miss){
	// set i and check if its the beginning of a continious part which sums up to miss
	for(int i = 0; i < input->length; i++){
		long res = input->content[i]; // accumulator
		for(int j = i+1; j < input->length; j++){
			res += input->content[j];
			// check if acc matches
			if(res == miss){

				// search for the min/max in range input->content[i] - input->content[j]
				long min = LONG_MAX;
				long max = 0;
				for(int z = i; z < j; z++){
					if(input->content[z] < min){
						min = input->content[z];
					}
					if(input->content[z] > max){
						max = input->content[z];
					}
				}
				return min+max;
			}
		}
	}
	return -1;
}

int main() {
	long type;
	(void)type;

	long miss;

	struct long_content *input;
	input = read_file("./problems/day-09.dat", type);
	printf("Part1:\nFist number not matching the rule: %ld\n", part1(input, 25, &miss));

	printf("\nPart2:\nMin*Max in range summing up to the miss: %ld\n", part2(input, miss));
	free_file(input);
	return EXIT_SUCCESS;
}
