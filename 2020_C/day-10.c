#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int compare(const void * a, const void * b){
	return *(int*)a - *(int*)b;
}

int part1(struct int_content* input){
	int last = 0;
	int amount1 = 0;
	int amount3 = 0;
	for(int i=0; i < input->length; i++){
		/* printf("Use %d to convert from %d to %d with step %d\n", input->content[i], last, input->content[i]+last, input->content[i]-last); */
		if(input->content[i] - last == 3){
			last = input->content[i];
			amount3++;
		} else if(input->content[i] - last == 2){
			last = input->content[i];
		} else if(input->content[i] - last == 1){
			last = input->content[i];
			amount1++;
		}
	}
	amount3++; // built-in adaper
	return amount1*amount3;
}

// since it is unconveniant to have different indices to work on (arr and input->content) the connection to the airplane
// isn't in the array but got an own if statement
long part2(struct int_content* input){
	// create an array to store the amount of ways to an element
	long* arr = malloc(sizeof(long)*input->length);

	arr[0] = 1; // position 1 has to have only one connection to the socket

	// for each number check if the three numbers below (if present) can read this number
	// and if so add the possible ways to numbers below to the possibilities for the current element
	for(int i=1; i < input->length; i++){
		arr[i] = 0;
		if(input->content[i] - input->content[i-1] <= 3){
			arr[i] += arr[i-1];
		}
		if(i-2 >= 0 && input->content[i] - input->content[i-2] <= 3){
			arr[i] += arr[i-2];
		}
		if(i-3 >= 0 && input->content[i] - input->content[i-3] <= 3){
			arr[i] += arr[i-3];
		}
		if(input->content[i] <= 3){
			arr[i] += 1;
		}
	}

	// return the possibilities to get to the last element
	return arr[input->length-1];
}

int main() {
	int type;
	(void)type;

	struct int_content *input;
	input = read_file("./problems/day-10.dat", type);
	qsort(input->content, input->length, sizeof(int), compare);

	printf("Part1:\n#3J * #1J in chain: %d\n", part1(input));

	printf("\nPart2:\nNumber of possibilities to connect: %ld\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
