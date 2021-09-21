#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "./read_file.h"
#include "queue.h"

struct cup {
  int label;
  TAILQ_ENTRY(cup) link;
};
TAILQ_HEAD(clock, cup);

struct cup* find(struct clock* clock, int label){
	struct cup* i = NULL;
	TAILQ_FOREACH(i, clock, link){
		if(i->label == label) return i;
	}
	return NULL;
}

void print(struct clock* clock, struct cup* active){
	struct cup* i = NULL;
	TAILQ_FOREACH(i, clock, link){
		if(i == active){
			printf("(%d), ", i->label);
		} else {
			printf("%d, ", i->label);
		}
	}
	printf("\n");
}

void printFinal(struct clock* clock){
	struct cup* i = NULL;
	_Bool printing = 0;
	TAILQ_FOREACH(i, clock, link){
		if(printing)
			printf("%d", i->label);
		if(i->label == 1) printing = 1;
	}
	TAILQ_FOREACH(i, clock, link){
		if(i->label == 1) break;
		printf("%d", i->label);
	}
	printf("\n");
}

struct cup* step(struct clock* clock, struct cup* active, int min, int max){
	// remove taken cups
	struct cup* n1 = TAILQ_NEXT(active, link);
	if(n1 == NULL) n1 = TAILQ_FIRST(clock);

	struct cup* n2 = TAILQ_NEXT(n1, link);
	if(n2 == NULL) n2 = TAILQ_FIRST(clock);

	struct cup* n3 = TAILQ_NEXT(n2, link);
	if(n3 == NULL) n3 = TAILQ_FIRST(clock);

	TAILQ_REMOVE(clock, n1, link); // loop
	TAILQ_REMOVE(clock, n2, link); // loop
	TAILQ_REMOVE(clock, n3, link); // loop

	// search the destination cup
	int dst_l = active->label - 1;
	struct cup* dst = NULL;
	while(dst == NULL){
		if(dst_l != n1->label && dst_l != n2->label && dst_l != n3->label)
			dst = find(clock, dst_l);
		dst_l--;
		if(dst_l < min) dst_l = max;
	}

	// insert the removed cups at the destination
	TAILQ_INSERT_AFTER(clock, dst, n1, link); // loop?
	TAILQ_INSERT_AFTER(clock, n1, n2, link); // loop?
	TAILQ_INSERT_AFTER(clock, n2, n3, link); // loop?

	// get the next active cup
	if(TAILQ_NEXT(active, link) != NULL)
		return TAILQ_NEXT(active, link);
	return TAILQ_FIRST(clock);
}

int part1(struct string_content* input){
	int len = strlen(input->content[0]);
	struct clock* clock = malloc(sizeof(struct clock));
	TAILQ_INIT(clock);
	int min = INT_MAX; int max = INT_MIN;
	for(int i = 0; i < len; i++){
		struct cup* cup = malloc(sizeof(struct cup));
		cup->label = input->content[0][i] - '0';
		TAILQ_INSERT_TAIL(clock, cup, link);
		if (cup->label < min)
			min = cup->label;
		if (cup->label > max)
			max = cup->label;
	}

	struct cup* active = TAILQ_FIRST(clock);
	for(int i = 0; i < 100; ++i){
		/* print(clock, active); */
		active = step(clock, active, min, max);
		/* print(clock, active); */
		/* printf("\n"); */
	}
	printFinal(clock);
	return 0;
}

#define size 1000000 // 1 000 000

#define rounds 10000000 // 10 000 000
// #define rounds 1000 // 1 000

int part2(struct string_content* input){
	int len = strlen(input->content[0]);
	struct clock* clock = malloc(sizeof(struct clock));
	TAILQ_INIT(clock);
	int min = INT_MAX; int max = INT_MIN;
	for(int i = 0; i < len; i++){
		struct cup* cup = malloc(sizeof(struct cup));
		cup->label = input->content[0][i] - '0';
		TAILQ_INSERT_TAIL(clock, cup, link);
		if (cup->label < min)
			min = cup->label;
		if (cup->label > max)
			max = cup->label;
	}
	for(int i = max; i <= max+(size-len); i++){
		struct cup* cup = malloc(sizeof(struct cup));
		cup->label = i;
		TAILQ_INSERT_TAIL(clock, cup, link);
	}
	max = max+(size-len);

	struct cup* active = TAILQ_FIRST(clock);

	for(int i = 0; i < rounds; ++i){
		if(i % (rounds/1000) == 0)
			printf("Progress: %d of %d\n", i / (rounds/1000), rounds / (rounds/1000));
		/* print(clock, active); */
		active = step(clock, active, min, max);
		/* print(clock, active); */
		/* printf("\n"); */
	}
	struct cup* one = find(clock, 1);

	struct cup* two = TAILQ_NEXT(one, link);
	if(two == NULL) two = TAILQ_FIRST(clock);

	struct cup* three = TAILQ_NEXT(two, link);
	if(three == NULL) three = TAILQ_FIRST(clock);

	printf("Fst after 1: %d\n", two->label);
	printf("Snd after 1: %d\n", three->label);
	return 0;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-23.dat.testing", type);
	printf("Part1:\n%d\n", part1(input));
	free_file(input);

	input = read_file("./day-23.dat.testing", type);
	printf("\nPart2:\n%d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
