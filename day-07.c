#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

#include <bsd/sys/tree.h>
#include <bsd/sys/queue.h>

#include <regex.h>

struct element_list {
	char* from;
	int amount;
	SLIST_ENTRY(element_list) link;
};
SLIST_HEAD(from_list, element_list);

struct element_tree {
	struct from_list* from;
	char* to;
	int amount;
	RB_ENTRY(element_tree) link;
};
int cmp(struct element_tree *a, struct element_tree *b){
	return strcmp(a->to, b->to);
}
RB_HEAD(from_to_map, element_tree);
RB_PROTOTYPE(from_to_map, element_tree, link, cmp)
RB_GENERATE(from_to_map, element_tree, link, cmp)

void
print_tree(struct element_tree *n)
{
	struct element_tree *left, *right;

	if (n == NULL) {
		printf("nil");
		return;
	}
	left = RB_LEFT(n, link);
	right = RB_RIGHT(n, link);
	if (left == NULL && right == NULL)
		printf("%s", n->to);
	else {
		printf("%s(", n->to);
		print_tree(left);
		printf(",");
		print_tree(right);
		printf(")");
	}
}

regex_t matcher;
regmatch_t pmatch[1];
void parse_to(char* to, struct from_to_map* map, char* from){
	regcomp(&matcher, " (bag(|s)?(, |\.))", REG_EXTENDED);
	char* left = to;

	// loop while current one still maps to something
	while(*left != '\0'){
		int num = atoi(strtok_r(left, " ", &left));

		// get at which place the to segment ends
		(void) regexec(&matcher, left, 1, pmatch, 0);

		// null terminate the to segment
		left[pmatch->rm_so] = '\0';
		printf("Num: %d\tto: %s\n", num, left);

		// insert to segment into the map
		struct element_tree* ele = malloc(sizeof(struct element_tree));
		*ele = (struct element_tree) {.to = left};
		struct element_tree* ret = RB_INSERT(from_to_map, map, ele);
		if(ret == NULL){
			// successfully inserted
			struct from_list* head = malloc(sizeof(struct from_list));
			ele->from = head;
			SLIST_INIT(head);
			struct element_list* segment = malloc(sizeof(struct element_list));
			segment->from = from;
			SLIST_INSERT_HEAD(head, segment, link);
		} else {
			// ele was already contained
			free(ele);
			struct element_list* segment = malloc(sizeof(struct element_list));
			segment->from = from;
			SLIST_INSERT_HEAD(ret->from, segment, link);
		}

		// go to the next segment
		left += pmatch->rm_eo;
	}
}

int rec_count(char* to, struct from_to_map* map, struct from_to_map* possible){
	struct element_list* z;

	struct element_tree find = {.to = to};
	struct element_tree* res = RB_FIND(from_to_map, map, &find);
	if(res == NULL){
		return 0;
	}

	SLIST_FOREACH(z, res->from, link){
		printf("%s\n", z->from);
		struct element_tree* ele = malloc(sizeof(struct element_tree));
		*ele = (struct element_tree) {.to = z->from};
		struct element_tree* ret = RB_INSERT(from_to_map, possible, ele);
		print_tree(RB_ROOT(possible));
		if(ret != NULL){
			// ele was already contained
			free(ele);
		}
		rec_count(z->from, map, possible);
	}
	return 0;
}

int part1(struct string_content* input){
	struct from_to_map map; // map which maps from to
	RB_INIT(&map);
	for(int i=0; i < input->length; i++){
		char* bags_contain = strstr(input->content[i], " bags contain ");
		*bags_contain = '\0';
		bags_contain += 14;
		printf("from: \"%s\" to: \"%s\"\n", input->content[i], bags_contain);

		parse_to(bags_contain, &map, input->content[i]);

		print_tree(RB_ROOT(&map));
		printf("\n\n");
	}
	struct element_tree* i;
	struct element_list* z;

	printf("\n");
	RB_FOREACH(i, from_to_map, &map){
		printf("to: \"%s\" ", i->to);
		SLIST_FOREACH(z, i->from, link){
			printf("from: \"%s\" ", z->from);
		}
		printf("\n");
	}
	printf("\n");


	struct from_to_map possible;
	RB_INIT(&possible);

	rec_count("shiny gold", &map, &possible);

	int counter = 0;
	printf("\n\nPossibilities:\n");
	RB_FOREACH(i, from_to_map, &possible){
		printf("%s, ", i->to);
		counter++;
	}
	printf("\n");
	return counter;
}


regex_t matcher;
regmatch_t pmatch[1];
void parse_to2(char* to, struct from_list* head){
	regcomp(&matcher, " (bag(|s)?(, |\.))", REG_EXTENDED);
	char* left = to;

	// loop while current one still maps to something
	while(*left != '\0'){
		int num = atoi(strtok_r(left, " ", &left));

		// get at which place the to segment ends
		(void) regexec(&matcher, left, 1, pmatch, 0);

		// null terminate the to segment
		left[pmatch->rm_so] = '\0';
		printf("Num: %d\tto: %s\n", num, left);

		// insert to segment into the map
		struct element_list* segment = malloc(sizeof(struct element_list));
		segment->from   = left;
		segment->amount = num;
		SLIST_INSERT_HEAD(head, segment, link);

		// go to the next segment
		left += pmatch->rm_eo;
	}
}


int rec_count2(char* to, struct from_to_map* map){
	struct element_list* z;

	struct element_tree find = {.to = to};
	struct element_tree* res = RB_FIND(from_to_map, map, &find);
	if(res == NULL){
		return 1;
	}

	int ret = 0;
	SLIST_FOREACH(z, res->from, link){
		if(z->amount == 0){
			continue;
		}
		printf("%d\n", z->amount);
		ret += z->amount * rec_count2(z->from, map);
	}
	return ret+1;
}

int part2(struct string_content* input){
	struct from_to_map map; // map which maps from to
	RB_INIT(&map);
	for(int i=0; i < input->length; i++){
		char* bags_contain = strstr(input->content[i], " bags contain ");
		*bags_contain = '\0';
		bags_contain += 14;
		printf("from: \"%s\" to: \"%s\"\n", input->content[i], bags_contain);

		// insert to segment into the map
		struct element_tree* ele = malloc(sizeof(struct element_tree));
		*ele = (struct element_tree) {.to = input->content[i]};
		struct element_tree* ret = RB_INSERT(from_to_map, &map, ele);
		if(ret == NULL){
			// successfully inserted
			struct from_list* head = malloc(sizeof(struct from_list));
			ele->from = head;
			SLIST_INIT(head);
			parse_to2(bags_contain, head);
		} else {
			// ele was already contained
			free(ele);
			parse_to2(bags_contain, ret->from);
		}

		print_tree(RB_ROOT(&map));
		printf("\n\n");
	}

	struct element_tree* i;
	struct element_list* z;

	printf("\n");
	RB_FOREACH(i, from_to_map, &map){
		printf("to: \"%s\" ", i->to);
		SLIST_FOREACH(z, i->from, link){
			printf("from: \"%s\"(%d) ", z->from, z->amount);
		}
		printf("\n");
	}
	printf("\n");

	return rec_count2("shiny gold", &map) - 1;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input = read_file("./day-07.dat", type);
	/* printf("Part1:\n%d\n", part1(input)); */
	/* free_file(input); */
	
	/* input = read_file("./day-07.dat.testing", type); */
	printf("\nPart2:\n%d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
