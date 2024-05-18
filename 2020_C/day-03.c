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

int part1(struct string_content* input, int change_x, int change_y, int print){
	int x = 0;
	int y = 0; // amount of lines is the height of the field

	int x_max = strlen(input->content[0]);

	int counter_trees = 0;
	int counter_empty = 0;
	while(y < input->length){
		if (print){
			printf("x:%d y:%d -> %c\t", x, y, input->content[y][x]);
		}
		if(input->content[y][x] == '#'){
			if (print){
				printf("Tree\n");
			}
			counter_trees++;
		} else {
			if (print){
				printf("no Tree\n");
			}
			counter_empty++;
		}
		y += change_y;
		x = (x + change_x) % x_max;
	}
	return counter_trees;
}

int part2(struct string_content* input){
	long ret = 1;
	ret *= part1(input, 1, 1, 0);
	ret *= part1(input, 3, 1, 0);
	ret *= part1(input, 5, 1, 0);
	ret *= part1(input, 7, 1, 0);
	ret *= part1(input, 1, 2, 0);

	return ret;
}

int main() {
	char* type;
	(void)type;

	struct string_content* input = read_file("./problems/day-03.dat", type);
	printf("Part1\nTrees: %d\n", part1(input, 3, 1, 0));

	printf("\nPart2\nTrees: %d\n", part2(input));
	free_file(input);

	return EXIT_SUCCESS;
}
