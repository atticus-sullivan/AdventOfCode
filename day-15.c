#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <bsd/sys/tree.h>

#include "./read_file.h"

struct element_tree {
	int number; // key
	int last_spoken;
	int last_last_spoken;
	RB_ENTRY(element_tree) link;
};
int cmp(struct element_tree *a, struct element_tree *b){
	return (a->number < b->number ? -1 : a->number > b->number);
}
RB_HEAD(spoken, element_tree);
RB_PROTOTYPE(spoken, element_tree, link, cmp)
RB_GENERATE(spoken, element_tree, link, cmp)


// FIXME: last_spoken is not nessacary (as last_spoken will always be i-1/i)
// TODO according to perf, the functions that take the most time are the one of the rb-tree
// TODO reduce lookups, since struct has been fetched in the last iteration
int part(struct int_content* input, int max){
	struct spoken spoken;
	RB_INIT(&spoken);

	int i;
	int first = 1;
	for(i=0; i < input->length; i++){
		struct element_tree* x = malloc(sizeof(struct element_tree));
		*x = (struct element_tree) {.number = input->content[i], .last_last_spoken = -1, .last_spoken = i};
		RB_INSERT(spoken, &spoken, x);
		/* printf("\n%d spoken\n", x->number); */
	}

	int number = input->content[input->length-1];
	for(; i < max; i++){
		// insert 0 if the last number was the first one
		if(first){
			number = 0;
		} else {
			struct element_tree find, *ret;
			find.number = number;
			ret = RB_FIND(spoken, &spoken, &find);
			assert(ret->last_last_spoken >= 0 && "last_last_spoken isn't set");
			assert(ret->last_spoken      >= 0 && "last_spoken isn't set");
			number = ret->last_spoken - ret->last_last_spoken;
		}

		// try to insert the new number, if already contained only update the last_spoken attributes
		struct element_tree* x = malloc(sizeof(struct element_tree));
		*x = (struct element_tree) {.number = number, .last_last_spoken = -1, .last_spoken = i};
		/* printf("\n%d spoken\n", x->number); */
		struct element_tree* ret = RB_INSERT(spoken, &spoken, x);
		if(ret != NULL){
			/* printf("already contained\n"); */
			ret->last_last_spoken = ret->last_spoken;
			ret->last_spoken = i;
			free(x);
			first = 0;
		} else {
			first = 1;
		}

	}

	struct element_tree *x, *y;
	RB_FOREACH_SAFE(x, spoken, &spoken, y){
		RB_REMOVE(spoken, &spoken, x);
		free(x);
	}
	
	return number;
}

int main() {
	char* type;
	(void)type;

	struct int_content *input;
	input = read_line_int("./day-15.dat");
	printf("Part1:\n%d\n", part(input, 2020));
	free_file(input);
	
	input = read_line_int("./day-15.dat");
	printf("\nPart2:\n%d\n", part(input, 30000000));
	free_file(input);
	return EXIT_SUCCESS;
}
