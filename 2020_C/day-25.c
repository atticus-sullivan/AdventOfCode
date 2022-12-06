#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

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
