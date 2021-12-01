#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

unsigned long parse_expr1(char** in){
#ifdef DEBUG_EXT
	printf("%s\n", *in);
#endif
	unsigned long acc = 0;
	if(**in == '('){
		(*in)++; // skip opening paranthesis
		acc = parse_expr1(in);
		(*in)++; // skip closing paranthesis
	} else {
		acc = strtol(*in, in, 10);
#ifdef DEBUG_EXT
		printf("\"%s\" remains\n", *in);
#endif
	}
	while(1){
#ifdef DEBUG_EXT
		printf("---\n%s\n---\n", *in);
#endif
		char* operator = strtok_r(*in, " ", in);
#ifdef DEBUG_EXT
		printf("\"%s\" remains with operator \"%s\"\n", *in, operator);
#endif
		int op2 = 0;
		if(**in == '('){
			(*in)++; // skip opening paranthesis
			op2 = parse_expr1(in);
			(*in)++; // skip closing paranthesis
		} else {
			op2 = strtol(*in, in, 10);
#ifdef DEBUG_EXT
			printf("\"%s\" remains\n", *in);
#endif
		}
#ifdef DEBUG_EXT
		printf("operand1: %d\n", acc);
		printf("operator: %s\n", operator);
		printf("operand2: %d\n", op2);
#endif
		if(*operator == '+'){
			acc += op2;
		} else {
			acc *= op2;
		}
#ifdef DEBUG_EXT
		printf("---\n%s\n---\n", *in);
#endif
		if(**in == '\0'|| **in == ')') break;
	}
	return acc;
}

unsigned long parse_expr2(char** in){
#ifdef DEBUG_EXT
	printf("-- enter parse-expr2\n");
#endif
	unsigned long acc = 0;

	// check for parantheses, otherwise read the first operand as accumulator
	if(**in == '('){
		(*in)++; // skip the opening paranthesis
		acc = parse_expr2(in);
		(*in)++; // skip the closing paranthesis
	} else {
		acc = strtol(*in, in, 10);
	}

	// go over expression
	while(1){
		// end if expression is over or the paranthesis ends
		if(**in == '\0' || **in == ')') break;
		char* operator = strtok_r(*in, " ", in); // read the operator
#ifdef DEBUG_EXT
		printf("\"%s\" remains with operator \"%s\"\n", *in, operator);
#endif
		// if '+', the operands instantly can be summed, if '*' the second operand still needs to get calculated before calculating
		unsigned long op2 = 0;
		if(*operator == '+'){
			op2 = strtol(*in, in, 10);
			acc += op2;
#ifdef DEBUG
			printf("op1: %lu, op2: %lu\n", acc, op2);
			printf("update acc to %lu\n", acc);
#endif
		} else{
			op2 = parse_expr2(in);
			acc *= op2;
#ifdef DEBUG_EXT
			printf("op1: %lu, op2: %lu\n", acc, op2);
			printf("update acc to %lu\n", acc);
#endif
			break;
		}

		// go one layer down if the next is a paranthesis (needs to get calculated first)
		// in this case strtol in the '+' branch will has calculate +0
		if(**in == '('){
			(*in)++; // skip the opening paranthesis
			op2 = parse_expr2(in);
			(*in)++; // skip the closing paranthesis
			if(*operator == '+'){
				acc += op2;
			} else {
				acc *= op2;
			}
#ifdef DEBUG_EXT
			printf("update acc to %lu\n", acc);
#endif
		}
		if(**in == '\0') break;
	}
	return acc;
}

unsigned long part1(struct string_content* input){
	unsigned long acc = 0;
	for(int i=0; i < input->length; i++){
		char* ptr = input->content[i];
#ifdef DEBUG
		printf("%s\n", ptr);
#endif
		unsigned long res = parse_expr1(&ptr);
		acc += res;
#ifdef DEBUG
		printf("-> %lu\n\n", res);
#endif
	}
	return acc;
}

unsigned long part2(struct string_content* input){
	unsigned long acc = 0;
	for(int i=0; i < input->length; i++){
		char* ptr = input->content[i];
#ifdef DEBUG
		printf("%s\n", ptr);
#endif
		unsigned long res = parse_expr2(&ptr);
		acc += res;
#ifdef DEBUG
		printf("-> %lu\n\n", res);
#endif
	}
	return acc;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-18.dat", type);
	printf("Part1:\nSum of the results (no predecence): %lu\n", part1(input));
	free_file(input);
	
	input = read_file("./day-18.dat", type);
	printf("\nPart2:\nSum of the results (predecence): %lu\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
