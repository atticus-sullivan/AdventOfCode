#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

inline int swap_nop_jmp(struct string_content* input, int i) __attribute__((always_inline));
inline int swap_nop_jmp(struct string_content* input, int i){
	if(!strncmp(input->content[i], "nop", 3)){
		input->content[i][0] = 'j';
		input->content[i][1] = 'm';
		return 1;
	} else if(!strncmp(input->content[i], "jmp", 3)){
		input->content[i][0] = 'n';
		input->content[i][1] = 'o';
		return 1;
	}
	return 0;
}

void exec_code(struct string_content* input, int ret[2]){
	// copy the pointers to work on
	char** code = malloc(sizeof(char*)*input->length);
	for(int i=0; i < input->length; i++){
		code[i] = input->content[i];
	}

	ret[0] = 0;
	char* op;
	int i = 0;
	while(i < input->length){
		if(code[i] == NULL){
			/* printf("i: %d  length: %d\n", i, input->length); */
			ret[1] = 0; // unsucessfull
			free(code);
			return;
		}
		op = code[i];
		code[i] = NULL;
		int num = atoi(op+4);
		if(!strncmp(op, "acc", 3)){
			ret[0] += num;
			/* printf("acc %d -> accumulator: %d\n", num, ret[0]); */
		}
		else if(!strncmp(op, "jmp", 3)){
			/* printf("jmp %d\n", num); */
			i += num;
			continue;
		}
		else if(!strncmp(op, "nop", 3)){
			/* printf("nop %d\n", num); */
		}
		i++;
	}
	ret[1] = 1; // sucessfull
	free(code);
	return;
}

int part1(struct string_content* input){
	int ret[2] = {0};
	exec_code(input, ret);
	return ret[0];
}

int part2(struct string_content* input){
	int ret[2] = {0};

	// FIXME not really nessacary to test the unchanged code
	exec_code(input, ret);
	if(ret[1]){
		return ret[0];
	}

	// check for each instruction if it can be changed and check if the prog terminates then
	for(int i=0; i < input->length; i++){
		if(swap_nop_jmp(input, i)){
			/* printf("possibly changed: %s\n", input->content[i]); */
			exec_code(input, ret);
			swap_nop_jmp(input, i);
			if(ret[1]){
				return ret[0];
			}
		}
	}
	return -1;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input = read_file("./day-08.dat", type);
	printf("Part1:\nAccumulator when the first instruction is executed twice: %d\n", part1(input));

	printf("\nPart2:\nAccumulator at the end of the program: %d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
