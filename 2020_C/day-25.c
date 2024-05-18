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

int get_loop_size(int subject, int key){
	int value = 1;
	int ret = 0;
	while(value != key){
		value *= subject;
		value %= 20201227;
		ret++;
	}
	return ret;
}

long gen_key(int subject, int loop){
	long value = 1;
	for(int i=0; i < loop; i++){
		value *= subject;
		value %= 20201227;
	}
	return value;
}

long part1(struct int_content* input){
	int loop0 = get_loop_size(7, input->content[0]);
	/* int loop1 = get_loop_size(7, input->content[1]); */

	long encKey = gen_key(input->content[1], loop0);
	return encKey;
}

int part2(struct int_content* input){
	(void)input;
	return 0;
}

int main() {
	int type;
	(void)type;

	struct int_content *input;
	input = read_file("./problems/day-25.dat", type);
	printf("Part1:\n%ld\n", part1(input));
	free_file(input);
	
	input = read_file("./problems/day-25.dat.testing", type);
	printf("\nPart2:\n%d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
