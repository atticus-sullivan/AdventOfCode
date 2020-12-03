#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

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

	struct int_content *input = read_file("./day-01.dat", type);
	printf("Part1\nx: %d\n", part1(input));
	free_file(input);

	input = read_file("./day-01.dat", type);
	printf("\nPart2\nx: %d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
