#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

inline void tokenize_pwd(char*, int*, int*, char*, char**) __attribute__((always_inline));

inline void tokenize_pwd(char* input, int* a, int* b, char* c, char** pwd){
		*a = atoi(strtok(input,"-"));
		*b = atoi(strtok(NULL," "));
		*c = *(strtok(NULL,":"));
		*pwd = strtok(NULL,"")+1;
}

int part1(struct string_content *input){
	int counter = 0;
	for(int i=0; i < input->length; i++){
		// tokenize <min>-<max> <char>: <pwd>
		int min = 0, max = 0;
		char ele = 0, *pwd = NULL;
		tokenize_pwd(input->content[i], &min, &max, &ele, &pwd);

		int count = 0;
		while(*(pwd) != '\0'){
			if(*pwd == ele){
				count++;
			}
			pwd++;
		}
		if(count >= min && count <= max){
			counter++;
			/* printf("Valid: %d-%d:%c %s\n", min, max, *ele, pwd); */
		}
	}
	return counter;
}

int part2(struct string_content *input){
	int counter = 0;
	for(int i=0; i < input->length; i++){
		// tokenize <min>-<max> <char>: <pwd>
		int a = 0, b = 0;
		char ele = 0, *pwd = NULL;
		tokenize_pwd(input->content[i], &a, &b, &ele, &pwd);

		if((pwd[a-1] == ele) ^ (pwd[b-1] == ele)){
			counter++;
			/* printf("Valid: %d-%d:%c %s\n", a, b, *ele, pwd); */
		}
	}
	return counter;
}

int main() {
	char* type;
	(void)type; // silence unnused but set warning

	struct string_content* input = read_file("./problems/day-02.dat", type);
	printf("Part1\nNum Valid: %d\n",part1(input));
	free_file(input);

	input = read_file("./problems/day-02.dat", type);
	printf("\nPart2\nNum Valid: %d\n",part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
