#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

void print_answers(unsigned int input[26]){
	printf("a: %d, b: %d, c: %d, d: %d, e: %d, f: %d, g: %d, h: %d, i: %d, j: %d, "\
			"k: %d, l: %d, m: %d, n: %d, o: %d, p: %d, q: %d, r: %d, s: %d, t: %d, "\
			"u: %d, v: %d, w: %d, x: %d, y: %d, z: %d\n\n", \
			input['a'-'a'], input['b'-'a'], input['c'-'a'], input['d'-'a'], input['e'-'a'], input['f'-'a'], input['g'-'a'], \
			input['h'-'a'], input['i'-'a'], input['j'-'a'], input['k'-'a'], input['l'-'a'], input['m'-'a'], input['n'-'a'], \
			input['o'-'a'], input['p'-'a'], input['q'-'a'], input['r'-'a'], input['s'-'a'], input['t'-'a'], input['u'-'a'], \
			input['v'-'a'], input['w'-'a'], input['x'-'a'], input['y'-'a'], input['z'-'a']);
}

inline int nx_checked_answer(unsigned int[26], unsigned int) __attribute__((always_inline));
inline int nx_checked_answer(unsigned int input[26], unsigned int min){
	int ret = 0;
	for(int i=0; i < 26; i++){
		if(input[i] >= min){
			ret += 1;
		}
	}
	return ret;
}

int part1(struct string_content* input){
	long ret = 0;
	for(int i=0; i < input->length; i++){ // while there is a group left
	unsigned int answers[26] = {0};
		while(strcmp(input->content[i], "") && i < input->length){ // while still in same group
			char* line = input->content[i];
			while(*line != '\0'){
				answers[(*line) - 'a'] += 1;
				line++; // go to the next char
			}
			i++; // go to the next line/person
		}
		/* print_answers(&answers); */
		ret += nx_checked_answer(answers, 1);
	}
	return ret;
}

int part2(struct string_content* input){
	long ret = 0;
	for(int i=0; i < input->length; i++){ // while there is a group left
	unsigned int answers[26] = {0};
	unsigned int group_size = 0;
		while(strcmp(input->content[i], "") && i < input->length){ // while still in same group
			group_size += 1;
			char* line = input->content[i];
			while(*line != '\0'){
				answers[(*line) - 'a'] += 1;
				line++; // go to the next char
			}
			i++; // go to the next line/person
		}
		/* printf("%d\n", group_size); */
		/* print_answers(&answers); */
		ret += nx_checked_answer(answers, group_size);
	}
	return ret;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input = read_file("./day-06.dat", type);
	printf("Part1:\nSum of checked boxes (or): %d\n", part1(input));

	printf("\nPart2:\nSum of checked boxes (and): %d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
