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

// returns -1 on out of bounds
// returns  0 on free seat
// returns  1 on occupied seat
int check_seat1(char** arr, int length, int width, int x, int y, int step_x, int step_y){
	if(x + step_x >= 0 && x + step_x < width  && y + step_y >= 0 && y + step_y < length){
		if(arr[y+step_y][x+step_x] == '#'){
			/* printf("0\n"); */
			return 1;
		} else {
			/* printf("1\n"); */
			return 0;
		}
	} else {
		// out of bounds
		/* printf("-1\n"); */
		return -1;
	}
}
int check_seat2(char** arr, int length, int width, int x, int y, int step_x, int step_y){
	while(x + step_x >= 0 && x + step_x < width  && y + step_y >= 0 && y + step_y < length){
		if(arr[y+step_y][x+step_x] == '#'){
			return 1;
		}
		if(arr[y+step_y][x+step_x] == 'L'){
			return 0;
		}
		x += step_x;
		y += step_y;
	}
	return 0;
}

int part(struct string_content* input1, struct string_content* input2, int tolerance, int check_seat(char**, int, int, int, int, int, int)){
	int width = strlen(input1->content[0]);
	/* print_file(input1); */

	// recognize if map changed
	int changed = 1;

	
	while(changed){
		changed = 0;
		// cycle over the lines and chars in lines
		for(int i=0; i < input1->length; i++){
			for(int j=0; j < width; j++){
				// check if someone will chose this seat in this iteration
				if (	(input1->content[i][j] == 'L') && // seat is empty
						/* // linear */
						check_seat(input1->content, input1->length, width, j, i, +0, +1) <= 0 &&
						check_seat(input1->content, input1->length, width, j, i, +0, -1) <= 0 &&
						check_seat(input1->content, input1->length, width, j, i, +1, +0) <= 0 &&
						check_seat(input1->content, input1->length, width, j, i, -1, +0) <= 0 &&
						/* // diagonal */
						check_seat(input1->content, input1->length, width, j, i, +1, -1) <= 0 &&
						check_seat(input1->content, input1->length, width, j, i, -1, -1) <= 0 &&
						check_seat(input1->content, input1->length, width, j, i, +1, +1) <= 0 &&
						check_seat(input1->content, input1->length, width, j, i, -1, +1) <= 0){
					/* printf("Seat %d,%d is surrounded by not occupied seats.\n", i, j); */
					input2->content[i][j] = '#'; // occupy seat
					changed = 1;
				}
				// check if this seat will be left in this iteration
				else if (input1->content[i][j] == '#'){
					int amount = 0;
					// linear
					if(check_seat(input1->content, input1->length, width, j, i, +0, +1) == 1){ /*if(i==2 && j == 7) printf("01\n");*/ amount++; }
					if(check_seat(input1->content, input1->length, width, j, i, +0, -1) == 1){ /*if(i==2 && j == 7) printf("02\n");*/ amount++; }
					if(check_seat(input1->content, input1->length, width, j, i, +1, +0) == 1){ /*if(i==2 && j == 7) printf("03\n");*/ amount++; }
					if(check_seat(input1->content, input1->length, width, j, i, -1, +0) == 1){ /*if(i==2 && j == 7) printf("04\n");*/ amount++; }
					// diagonal
					if(check_seat(input1->content, input1->length, width, j, i, +1, -1) == 1){ /*if(i==2 && j == 7) printf("05\n");*/ amount++; }
					if(check_seat(input1->content, input1->length, width, j, i, -1, -1) == 1){ /*if(i==2 && j == 7) printf("06\n");*/ amount++; }
					if(check_seat(input1->content, input1->length, width, j, i, +1, +1) == 1){ /*if(i==2 && j == 7) printf("07\n");*/ amount++; }
					if(check_seat(input1->content, input1->length, width, j, i, -1, +1) == 1){ /*if(i==2 && j == 7) printf("08\n");*/ amount++; }

					if(amount >= tolerance){
						input2->content[i][j] = 'L';
						changed = 1;
					}
				} // close else if
			} // close for loop j
		} // close for loop i

		// push update to input1
		for(int i=0; i < input1->length; i++){
			strcpy(input1->content[i], input2->content[i]);
		}
		/* printf("\n"); */
		/* print_file(input1); */

	} // end of while

	// count occupied places
	int counter = 0;
	for(int i=0; i < input1->length; i++){
		for(int j=0; j < width; j++){
			if(input1->content[i][j] == '#'){
				counter++;
			}
		}
	}
	return counter;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input1;
	struct string_content *input2;
	input1 = read_file("./problems/day-11.dat", type);
	input2 = read_file("./problems/day-11.dat", type);
	printf("Part1:\nAmount of occupied seats (Prediction 1): %d\n", part(input1, input2, 4, check_seat1));
	free_file(input1);
	free_file(input2);
	
	input1 = read_file("./problems/day-11.dat", type);
	input2 = read_file("./problems/day-11.dat", type);
	printf("\nPart2:\nAmount of occupied seats (Prediction 2): %d\n", part(input1,input2, 5, check_seat2));
	free_file(input1);
	free_file(input2);
	return EXIT_SUCCESS;
}
