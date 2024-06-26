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

void calc_coords(char* line, int max_row, int max_col, int ret[3]){
	int lst = max_row-1;
	int fst = 0;
	/* printf("\n%s\n", input->content[i]); */
	/* printf("%d - %d\n", fst, lst); */
	for(int r=0; r < 7; r++){
		if(line[r] == 'F'){
			lst -= (lst - fst)/2+1;
			/* printf("F -> %d - %d\n", fst, lst); */
		} else {
			fst += (lst - fst)/2+1;
			/* printf("B -> %d - %d\n", fst, lst); */
		}
	}
	ret[0] = lst;
	/* printf("%s -> Row: %d\n", input->content[i], lst); */
	lst = max_col-1;
	fst = 0;
	/* printf("%d - %d\n", fst, lst); */
	for(int c=0; c < 3; c++){
		// decode col
		if(line[c+7] == 'L'){
			lst -= (lst - fst)/2+1;
			/* printf("L -> %d - %d\n", fst, lst); */
		} else {
			fst += (lst - fst)/2+1;
			/* printf("R -> %d - %d\n", fst, lst); */
		}
	}
	ret[1] = lst;
	ret[2] = ret[0]*8 + ret[1];
}

int part1(struct string_content* input, int max_row, int max_col){
	int max = 0;
	for(int i=0; i < input->length; i++){
		int pos[3] = {0};
		calc_coords(input->content[i], max_row, max_col, pos);
		/* printf("%s -> row: %d  col: %d  id: %d\n\n", input->content[i], pos[0], pos[1], pos[2]); */
		if(max < pos[2]){
			max = pos[2];
		}
	}
	return max;
}

int part2(struct string_content* input, int max_row, int max_col){
	char *places = malloc((max_row*max_col) * sizeof(char));
	for(int i=0; i < (max_col*max_row); i++){
		places[i] = 0;
	}

	for(int i=0; i < input->length && i < max_col*max_row; i++){
		int pos[3] = {0};
		calc_coords(input->content[i], max_row, max_col, pos);
		places[pos[2]] = 1; // set place occupied
	}

	for(int i=1; i < max_col*max_row-1; i++){
		if(places[i-1] == 1 && places[i+1] == 1 && places[i] == 0){
			free(places);
			return i;
		}
	}
	free(places);
	return -1;
}

int main() {
	char* type;
	(void)type;

	struct string_content* input = read_file("./problems/day-05.dat", type);
	printf("Part1\nMax id: %d\n", part1(input, 128, 8));

	printf("\nPart2\nSearched ID: %d\n", part2(input, 128, 8));
	free_file(input);
	return EXIT_SUCCESS;
}
