#include <limits.h>
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

int part1(struct int_content* input){
	for(int i=0; i < input->length; i++){
		for(int z=0; z < input->length; z++){
			if(input->content[z]+input->content[i] == 2020){
				return input->content[i]*input->content[z];
			}
		}
	}
	return -1;
}

int part2(struct int_content* input){
	for(int i=0; i < input->length; i++){
		for(int z=0; z < input->length; z++){
			for(int x=0; x < input->length; x++){
				if(input->content[z]+input->content[i]+input->content[x] == 2020){
					return input->content[i]*input->content[z]*input->content[x];
				}
			}
		}
	}
	return -1;
}

int main(){
	int type = 0; // only for the generic
	(void)type; // silence unnused but set warning

	struct int_content *input = read_file("./problems/day-01.dat", type);
	printf("Part1\nx: %d\n", part1(input));

	printf("\nPart2\nx: %d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
