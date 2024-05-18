#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

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

// exmplanation how the direction is to be understood
/* enum direction{east=0, north=1, west=2, south=3}; */

int part1(struct string_content* input){
	int x = 0;
	int y = 0;
	int dir = 0;
	/* printf("Location: %d %d\n", x, y); */

	for(int i=0; i < input->length; i++){
		switch(*input->content[i]){
			case 'N': y += atoi(input->content[i]+1); break;
			case 'S': y -= atoi(input->content[i]+1); break;
			case 'E': x += atoi(input->content[i]+1); break;
			case 'W': x -= atoi(input->content[i]+1); break;
			case 'L': dir = ((dir + atoi(input->content[i]+1)/90) % 4 + 4) % 4; break;
			case 'R': dir = ((dir - atoi(input->content[i]+1)/90) % 4 + 4) % 4; break;
			case 'F':
					  switch(dir){
						  case 0: x += atoi(input->content[i]+1); break;
						  case 1: y += atoi(input->content[i]+1); break;
						  case 2: x -= atoi(input->content[i]+1); break;
						  case 3: y -= atoi(input->content[i]+1); break;
					  };
		};
		/* printf("%s\tLocation: %d %d \t Dir: %d\n", input->content[i], x, y, dir); */
	}
	return abs(x)+abs(y);
}

int part2(struct string_content* input){
	int x_ship = 0;
	int y_ship = 0;

	int x_way = 10;
	int y_way = 1;
	/* printf("Location Ship: %ld %ld\n", x_ship, y_ship); */
	/* printf("Location Way:  %ld %ld\n\n", x_way, y_way); */

	for(int i=0; i < input->length; i++){
		int num; // used as temporary storage
		switch(*input->content[i]){
			case 'N': y_way += atoi(input->content[i]+1); break;
			case 'S': y_way -= atoi(input->content[i]+1); break;
			case 'E': x_way += atoi(input->content[i]+1); break;
			case 'W': x_way -= atoi(input->content[i]+1); break;

			case 'L':
					switch(atoi(input->content[i]+1)/90){
						case 1:
							num = x_way;
							x_way = -y_way;
							y_way = num;
							break;
						case 2:
							x_way = -x_way;
							y_way = -y_way;
							break;
						case 3:
							num = x_way;
							x_way = y_way;
							y_way = -num;
							break;
						default:
							printf("Error2\n");
					};
					break;
			case 'R':
					switch(atoi(input->content[i]+1)/90){
						case 1:
							num = x_way;
							x_way = y_way;
							y_way = -num;
							break;
						case 2:
							x_way = -x_way;
							y_way = -y_way;
							break;
						case 3:
							num = x_way;
							x_way = -y_way;
							y_way = num;
							break;
						default:
							printf("Error1\n");
					};
					break;
			case 'F':
					num = atoi(input->content[i]+1);
					x_ship += x_way * num;
					y_ship += y_way * num;
					break;
		};
		/* printf("%s\tLocation Ship: %ld %ld\n", input->content[i], x_ship, y_ship); */
		/* printf("%s\tLocation Way:  %ld %ld\n\n", input->content[i], x_way, y_way); */
	}
	return abs(x_ship)+abs(y_ship);
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./problems/day-12.dat", type);
	printf("Part1:\nManhatten distance: %d\n", part1(input));
	printf("\nPart2:\nManhatten distance: %d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}

// bad try calculating with sin/cos does not work
/* dist = sqrt(x_way*x_way + y_way*y_way); */
/* deg = asin(y_way/dist); */
/* printf("Dist: %f\tdeg vorher: %f\t", dist, deg); */
/* deg -= (double)atoi(input->content[i]+1) * M_PI/180; */
/* printf("deg nachher: %f\n", deg); */

/* y_way = lround(sin(deg)*dist); */
/* x_way = lround(cos(deg)*dist); */
