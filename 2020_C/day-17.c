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

// TODO how to solve in infinite plane? (no fixed array)

#define ARR_SIZ3 30
#define ARR_SIZ4 28 // TODO valgrind reports invalid read at weird places (sanitzers not) if the array is such big

int neighbours3[26][3] = {
	{-1, -1, -1},
	{-1, -1,  0},
	{-1, -1,  1},
	{-1,  0, -1},
	{-1,  0,  0},
	{-1,  0,  1},
	{-1,  1, -1},
	{-1,  1,  0},
	{-1,  1,  1},
	{ 0, -1, -1},
	{ 0, -1,  0},
	{ 0, -1,  1},
	{ 0,  0, -1},
	{ 0,  0,  1},
	{ 0,  1, -1},
	{ 0,  1,  0},
	{ 0,  1,  1},
	{ 1, -1, -1},
	{ 1, -1,  0},
	{ 1, -1,  1},
	{ 1,  0, -1},
	{ 1,  0,  0},
	{ 1,  0,  1},
	{ 1,  1, -1},
	{ 1,  1,  0},
	{ 1,  1,  1},
};

int neighbours4[80][4] = {
	{-1, -1, -1, -1},
	{-1, -1, -1,  0},
	{-1, -1, -1,  1},
	{-1, -1,  0, -1},
	{-1, -1,  0,  0},
	{-1, -1,  0,  1},
	{-1, -1,  1, -1},
	{-1, -1,  1,  0},
	{-1, -1,  1,  1},
	{-1,  0, -1, -1},
	{-1,  0, -1,  0},
	{-1,  0, -1,  1},
	{-1,  0,  0, -1},
	{-1,  0,  0,  0},
	{-1,  0,  0,  1},
	{-1,  0,  1, -1},
	{-1,  0,  1,  0},
	{-1,  0,  1,  1},
	{-1,  1, -1, -1},
	{-1,  1, -1,  0},
	{-1,  1, -1,  1},
	{-1,  1,  0, -1},
	{-1,  1,  0,  0},
	{-1,  1,  0,  1},
	{-1,  1,  1, -1},
	{-1,  1,  1,  0},
	{-1,  1,  1,  1},
	{ 0, -1, -1, -1},
	{ 0, -1, -1,  0},
	{ 0, -1, -1,  1},
	{ 0, -1,  0, -1},
	{ 0, -1,  0,  0},
	{ 0, -1,  0,  1},
	{ 0, -1,  1, -1},
	{ 0, -1,  1,  0},
	{ 0, -1,  1,  1},
	{ 0,  0, -1, -1},
	{ 0,  0, -1,  0},
	{ 0,  0, -1,  1},
	{ 0,  0,  0, -1},
	{ 0,  0,  0,  1},
	{ 0,  0,  1, -1},
	{ 0,  0,  1,  0},
	{ 0,  0,  1,  1},
	{ 0,  1, -1, -1},
	{ 0,  1, -1,  0},
	{ 0,  1, -1,  1},
	{ 0,  1,  0, -1},
	{ 0,  1,  0,  0},
	{ 0,  1,  0,  1},
	{ 0,  1,  1, -1},
	{ 0,  1,  1,  0},
	{ 0,  1,  1,  1},
	{ 1, -1, -1, -1},
	{ 1, -1, -1,  0},
	{ 1, -1, -1,  1},
	{ 1, -1,  0, -1},
	{ 1, -1,  0,  0},
	{ 1, -1,  0,  1},
	{ 1, -1,  1, -1},
	{ 1, -1,  1,  0},
	{ 1, -1,  1,  1},
	{ 1,  0, -1, -1},
	{ 1,  0, -1,  0},
	{ 1,  0, -1,  1},
	{ 1,  0,  0, -1},
	{ 1,  0,  0,  0},
	{ 1,  0,  0,  1},
	{ 1,  0,  1, -1},
	{ 1,  0,  1,  0},
	{ 1,  0,  1,  1},
	{ 1,  1, -1, -1},
	{ 1,  1, -1,  0},
	{ 1,  1, -1,  1},
	{ 1,  1,  0, -1},
	{ 1,  1,  0,  0},
	{ 1,  1,  0,  1},
	{ 1,  1,  1, -1},
	{ 1,  1,  1,  0},
	{ 1,  1,  1,  1},
};

void print_arr3(int arr[ARR_SIZ3][ARR_SIZ3][ARR_SIZ3]){
	return;
	for(int z=-ARR_SIZ3/2; z < ARR_SIZ3/2; z++){
		printf("\nz=%d\n", z);
		for(int x=-ARR_SIZ3/2; x < ARR_SIZ3/2; x++){
			for(int y=-ARR_SIZ3/2; y < ARR_SIZ3/2; y++){
				printf("%d ", arr[x+ARR_SIZ3/2][y+ARR_SIZ3/2][z+ARR_SIZ3/2]);
			}
			printf("\n");
		}
	}
}

void print_arr4(int arr[ARR_SIZ4][ARR_SIZ4][ARR_SIZ4][ARR_SIZ4]){
	return;
	for(int w=-ARR_SIZ4/2; w < ARR_SIZ4/2; w++){
		for(int z=-ARR_SIZ4/2; z < ARR_SIZ4/2; z++){
			printf("\nz=%d, w=%d\n", z, w);
			for(int x=-ARR_SIZ4/2; x < ARR_SIZ4/2; x++){
				for(int y=-ARR_SIZ4/2; y < ARR_SIZ4/2; y++){
					printf("%d ", arr[x+ARR_SIZ4/2][y+ARR_SIZ4/2][z+ARR_SIZ4/2][w+ARR_SIZ4/2]);
				}
				printf("\n");
			}
		}
	}
}

int part1(struct string_content* input){
	int grid[ARR_SIZ3][ARR_SIZ3][ARR_SIZ3] = {0};
	int grid2[ARR_SIZ3][ARR_SIZ3][ARR_SIZ3] = {0};

	for(int i=0; i < input->length; i++){
		for(size_t j=0; j < strlen(input->content[i]); j++){
			if(input->content[i][j] == '#'){
				grid2[i+ARR_SIZ3/2][j+ARR_SIZ3/2][ARR_SIZ3/2] = 1;
			}
		}
	}

	print_arr3(grid2);

	for(int cycle=0; cycle < 6; cycle++){
		// sync the both grids
		for(int z=0; z < ARR_SIZ3; z++){
			for(int x=0; x < ARR_SIZ3; x++){
				for(int y=0; y < ARR_SIZ3; y++){
					grid[x][y][z] = grid2[x][y][z];
				}
			}
		}
		for(int x=-ARR_SIZ3/2; x < ARR_SIZ3/2; x++){
			for(int y=-ARR_SIZ3/2; y < ARR_SIZ3/2; y++){
				for(int z=-ARR_SIZ3/2; z < ARR_SIZ3/2; z++){
					int counter = 0;
					for(int i=0; i < 26; i++){
						int x_neigh = x + neighbours3[i][0];
						int y_neigh = y + neighbours3[i][1];
						int z_neigh = z + neighbours3[i][2];
						if(
								x_neigh < -ARR_SIZ3/2 || x_neigh >= ARR_SIZ3/2 ||
								y_neigh < -ARR_SIZ3/2 || y_neigh >= ARR_SIZ3/2 ||
								z_neigh < -ARR_SIZ3/2 || z_neigh >= ARR_SIZ3/2) continue;
						if(grid[x_neigh+ARR_SIZ3/2][y_neigh+ARR_SIZ3/2][z_neigh+ARR_SIZ3/2] == 1){
							counter++;
						}
					}
					if(!(grid[x+ARR_SIZ3/2][y+ARR_SIZ3/2][z+ARR_SIZ3/2] == 1 && (counter == 2 || counter == 3))){
						grid2[x+ARR_SIZ3/2][y+ARR_SIZ3/2][z+ARR_SIZ3/2] = 0;
					}
					if(grid[x+ARR_SIZ3/2][y+ARR_SIZ3/2][z+ARR_SIZ3/2] == 0 && counter == 3){
						grid2[x+ARR_SIZ3/2][y+ARR_SIZ3/2][z+ARR_SIZ3/2] = 1;
					}
				}
			}
		}
		/* printf("\n\nAfter Cycle %d\n", cycle); */
		print_arr3(grid2);
	}

	int acc = 0;
	for(int z=-ARR_SIZ3/2; z < ARR_SIZ3/2; z++){
		for(int x=-ARR_SIZ3/2; x < ARR_SIZ3/2; x++){
			for(int y=-ARR_SIZ3/2; y < ARR_SIZ3/2; y++){
				acc += grid2[x+ARR_SIZ3/2][y+ARR_SIZ3/2][z+ARR_SIZ3/2];
			}
		}
	}
	return acc;
}

int part2(struct string_content* input){
	int grid[ARR_SIZ4][ARR_SIZ4][ARR_SIZ4][ARR_SIZ4] = {0};
	int grid2[ARR_SIZ4][ARR_SIZ4][ARR_SIZ4][ARR_SIZ4] = {0};

	for(int i=0; i < input->length; i++){
		for(size_t j=0; j < strlen(input->content[i]); j++){
			if(input->content[i][j] == '#'){
				grid2[i+ARR_SIZ4/2][j+ARR_SIZ4/2][ARR_SIZ4/2][ARR_SIZ4/2] = 1;
			}
		}
	}

	print_arr4(grid2);

	for(int cycle=0; cycle < 6; cycle++){
		// sync the both grids
		for(int w=0; w < ARR_SIZ4; w++){
			for(int z=0; z < ARR_SIZ4; z++){
				for(int x=0; x < ARR_SIZ4; x++){
					for(int y=0; y < ARR_SIZ4; y++){
						grid[x][y][z][w] = grid2[x][y][z][w];
					}
				}
			}
		}
		for(int x=-ARR_SIZ4/2; x < ARR_SIZ4/2; x++){
			for(int y=-ARR_SIZ4/2; y < ARR_SIZ4/2; y++){
				for(int z=-ARR_SIZ4/2; z < ARR_SIZ4/2; z++){
					for(int w=-ARR_SIZ4/2; w < ARR_SIZ4/2; w++){
						int counter = 0;
						for(int i=0; i < 80; i++){
							int x_neigh = x + neighbours4[i][0];
							int y_neigh = y + neighbours4[i][1];
							int z_neigh = z + neighbours4[i][2];
							int w_neigh = w + neighbours4[i][3];
							if(
									x_neigh < -ARR_SIZ4/2 || x_neigh >= ARR_SIZ4/2 ||
									y_neigh < -ARR_SIZ4/2 || y_neigh >= ARR_SIZ4/2 ||
									z_neigh < -ARR_SIZ4/2 || z_neigh >= ARR_SIZ4/2 ||
									w_neigh < -ARR_SIZ4/2 || w_neigh >= ARR_SIZ4/2) continue;
							if(grid[x_neigh+ARR_SIZ4/2][y_neigh+ARR_SIZ4/2][z_neigh+ARR_SIZ4/2][w_neigh+ARR_SIZ4/2] == 1){
								counter++;
							}
						}
						/* if(z == -1 && w == -1){ */
						/* 	printf("counter: %d\n", counter); */
						/* } */
						if(!(grid[x+ARR_SIZ4/2][y+ARR_SIZ4/2][z+ARR_SIZ4/2][w+ARR_SIZ4/2] == 1 && (counter == 2 || counter == 3))){
							grid2[x+ARR_SIZ4/2][y+ARR_SIZ4/2][z+ARR_SIZ4/2][w+ARR_SIZ4/2] = 0;
						}
						if(grid[x+ARR_SIZ4/2][y+ARR_SIZ4/2][z+ARR_SIZ4/2][w+ARR_SIZ4/2] == 0 && counter == 3){
							grid2[x+ARR_SIZ4/2][y+ARR_SIZ4/2][z+ARR_SIZ4/2][w+ARR_SIZ4/2] = 1;
						}
					}
				}
			}
		}
		/* printf("\n\nAfter Cycle %d\n", cycle); */
		print_arr4(grid2);
	}

	int acc = 0;
	for(int z=-ARR_SIZ4/2; z < ARR_SIZ4/2; z++){
		for(int w=-ARR_SIZ4/2; w < ARR_SIZ4/2; w++){
			for(int x=-ARR_SIZ4/2; x < ARR_SIZ4/2; x++){
				for(int y=-ARR_SIZ4/2; y < ARR_SIZ4/2; y++){
					acc += grid2[x+ARR_SIZ4/2][y+ARR_SIZ4/2][z+ARR_SIZ4/2][w+ARR_SIZ4/2];
				}
			}
		}
	}
	return acc;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./problems/day-17.dat", type);
	printf("Part1:\nAmount of active Cubes (3d): %d\n", part1(input));
	printf("\nPart2:\nAmount of active Hypercubes (4d): %d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
