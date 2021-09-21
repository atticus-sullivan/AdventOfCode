#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "./read_file.h"
#include "queue.h"
#include "tree.h"


struct cup {
  int label;
  TAILQ_ENTRY(cup) link;
};
TAILQ_HEAD(clock, cup);

void insert_after(struct clock* head, struct cup* listelem, struct cup* elem){
	TAILQ_INSERT_AFTER(head, listelem, elem, link);
}

void insert_tail(struct clock* head, struct cup* elem){
	/* printf("inserting %d\n", elem->label); */
	TAILQ_INSERT_TAIL(head, elem, link);
}

void remove_tailq(struct clock* head, struct cup* elem){
	TAILQ_REMOVE(head, elem, link);
}

struct cup* find(struct cup* map, int label){
	if(label <= 0) return NULL;
	return map + label;
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
	printf("\n");
}

struct clock* step(struct clock* clock, struct cup* map, int min, int max, int rounds, int steps){
	struct cup* active = TAILQ_FIRST(clock);
	/* print(clock, active); */
	for(int i = 0; i < rounds; ++i){
		if(i % (rounds/steps) == 0)
			printf("Progress: %d of %d\n", i / (rounds/steps), steps);
		// remove taken cups
		struct cup* n1 = TAILQ_NEXT(active, link);
		if(n1 == NULL) n1 = TAILQ_FIRST(clock);

		struct cup* n2 = TAILQ_NEXT(n1, link);
		if(n2 == NULL) n2 = TAILQ_FIRST(clock);

		struct cup* n3 = TAILQ_NEXT(n2, link);
		if(n3 == NULL) n3 = TAILQ_FIRST(clock);

		remove_tailq(clock, n1);
		remove_tailq(clock, n2);
		remove_tailq(clock, n3);

		// search the destination cup
		int dst_l = active->label - 1;
		struct cup* dst = NULL;
		while(dst == NULL){
			if(dst_l != n1->label && dst_l != n2->label && dst_l != n3->label)
				dst = find(map, dst_l);
			dst_l--;
			if(dst_l < min) dst_l = max;
		}

		// insert the removed cups at the destination
		insert_after(clock, dst, n1);
		insert_after(clock, n1, n2);
		insert_after(clock, n2, n3);

		/* print(clock, active); */
		/* printf("\n"); */

		// get the next active cup
		if(TAILQ_NEXT(active, link) != NULL)
			active = TAILQ_NEXT(active, link);
		else
			active = TAILQ_FIRST(clock);
	}
	/* print(clock, active); */
	return clock;
}

int part1(struct string_content* input){
	int len = strlen(input->content[0]);

	struct clock* clock = malloc(sizeof(struct clock));
	TAILQ_INIT(clock);

	printf("make place for %d elems\n", len);
	struct cup* map = malloc((len+1) * sizeof(struct cup));

	int min = INT_MAX; int max = INT_MIN;
	for(int i = 0; i < len; i++){
		struct cup* cup = map + (input->content[0][i] - '0');
		cup->label = input->content[0][i] - '0';
		insert_tail(clock, cup);
		if (cup->label < min)
			min = cup->label;
		if (cup->label > max)
			max = cup->label;
	}

	step(clock, map, min, max, 100, 1);

	printFinal(clock);
	return 0;
}

#define size2 1000000 // 1 000 000
// #define size2 10000 // 10 000

#define rounds2 10000000 // 10 000 000
// #define rounds2 1000 // 1 000

# define steps2 10


// idea of using an array as some sort of map from https://bitbucket.org/ainwood87/aoc/src/master/day23x/main.c
long part2(struct string_content* input){
	int len = strlen(input->content[0]);

	struct clock* clock = malloc(sizeof(struct clock));
	TAILQ_INIT(clock);

	struct cup* map = malloc((size2+1) * sizeof(struct cup));

	int min = INT_MAX; int max = INT_MIN;
	for(int i = 0; i < len; i++){
		struct cup* cup = map + (input->content[0][i] - '0');
		cup->label = input->content[0][i] - '0';
		insert_tail(clock, cup);
		if (cup->label < min)
			min = cup->label;
		if (cup->label > max)
			max = cup->label;
	}
	for(int i = max+1; i <= max+(size2-len); i++){
		struct cup* cup = map + (i);
		cup->label = i;
		insert_tail(clock, cup);
	}
	max = max+(size2-len);
	step(clock, map, min, max, rounds2, steps2);

	struct cup* one = find(map, 1);

	struct cup* two = TAILQ_NEXT(one, link);
	if(two == NULL) two = TAILQ_FIRST(clock);

	struct cup* three = TAILQ_NEXT(two, link);
	if(three == NULL) three = TAILQ_FIRST(clock);

	printf("Fst after 1: %d\n", two->label);
	printf("Snd after 1: %d\n", three->label);
	return (long)two->label * three->label;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-23.dat", type);
	printf("Part1:\n%d\n", part1(input));

	input = read_file("./day-23.dat", type);
	printf("\nPart2:\n%ld\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
// TODO free DS
