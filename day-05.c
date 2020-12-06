#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

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
	short *places = malloc((max_row*max_col) * sizeof(short*));
	for(int i=0; i < (max_col*max_row); i++){
		places[i] = 0;
	}

	for(int i=0; i < input->length; i++){
		int pos[3] = {0};
		calc_coords(input->content[i], max_row, max_col, pos);
		places[pos[2]] = 1;
	}

	for(int i=0; i < max_row; i++){
		for(int z=1; z < max_col-1; z++){
			if(places[i*8+z-1] == 1 && places[i*8+z+1] == 1 && places[i*8+z] == 0){
				return i*8+z;
			}
		}
	}

	return -1;
}

int main() {
	char* type;
	(void)type;

	struct string_content* input = read_file("./day-05.dat", type);
	printf("Part1\nMax id: %d\n", part1(input, 128, 8));
	printf("\nPart2\nSearched ID: %d\n", part2(input, 128, 8));
	free_file(input);
	return EXIT_SUCCESS;
}
