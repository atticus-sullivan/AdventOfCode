#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

void part1(struct string_content *input){
	printf("Part1\n");
	int counter = 0;
	for(int i=0; i < input->length; i++){
		// tokenize <min>-<max> <char>: <pwd>
		char* min_c = strtok(input->content[i],"-");
		char* max_c = strtok(NULL," ");
		char* ele = strtok(NULL,":");
		char* pwd = strtok(NULL,"")+1;

		int min = atoi(min_c);
		int max = atoi(max_c);

		int count = 0;
		while(*(pwd) != '\0'){
			if(*pwd == *ele){
				count++;
			}
			pwd++;
		}
		if(count >= min && count <= max){
			counter++;
			/* printf("Valid: %d-%d:%c %s\n", min, max, *ele, pwd); */
		}
	}
	printf("%d\n", counter);
}

void part2(struct string_content *input){
	printf("Part2\n");
	int counter = 0;
	for(int i=0; i < input->length; i++){
		// tokenize <min>-<max> <char>: <pwd>
		char* a_c = strtok(input->content[i],"-");
		char* b_c = strtok(NULL," ");
		char* ele = strtok(NULL,":");
		char* pwd = strtok(NULL,"")+1;

		int a = atoi(a_c);
		int b = atoi(b_c);

		if((pwd[a-1] == *ele) ^ (pwd[b-1] == *ele)){
			counter++;
			/* printf("Valid: %d-%d:%c %s\n", a, b, *ele, pwd); */
		}
	}
	printf("%d\n", counter);
}

int main() {
	char* type;
	(void)type; // silence unnused but set warning

	struct string_content* input = read_file("./day-02.dat", type);
	part1(input);
	free_file(input);

	input = read_file("./day-02.dat", type);
	part2(input);
	free_file(input);
	return EXIT_SUCCESS;
}
