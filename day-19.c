#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./read_file.h"

#include "bsd/sys/tree.h"
#include "bsd/sys/queue.h"

struct element_rules {
	int rule_num; // key
	struct alt_head* alternatives;
	RB_ENTRY(element_rules) link;
};
int cmp_rules(struct element_rules *a, struct element_rules *b){
	return (a->rule_num < b->rule_num ? -1 : a->rule_num > b->rule_num);
}
RB_HEAD(rules_head, element_rules);
RB_PROTOTYPE(rules_head, element_rules, link, cmp_rules)
RB_GENERATE(rules_head, element_rules, link, cmp_rules)

struct element_alt {
	struct subrules_head* sub;
	STAILQ_ENTRY(element_alt) link;
};
STAILQ_HEAD(alt_head, element_alt);

struct subrules_ele {
	char literal;
	int next_i;
	struct element_rules* next;
	STAILQ_ENTRY(subrules_ele) link;
};
STAILQ_HEAD(subrules_head, subrules_ele);

struct element_indices {
	int index;
	STAILQ_ENTRY(element_indices) link;
};
STAILQ_HEAD(head_indices, element_indices);

void print_list(char* string, struct head_indices* indices, int indent){
#ifdef DEBUG
					printf("%*s%s: ", indent, "", string);
					struct element_indices* muell;
					STAILQ_FOREACH(muell, indices, link){
						printf("%d ", muell->index);
					}
					printf("\n");
#endif
}

void free_list(struct head_indices* indices){
	struct element_indices* i;
	struct element_indices* i_save;
	STAILQ_FOREACH_SAFE(i, indices, link, i_save){
		STAILQ_REMOVE(indices, i, element_indices, link);
		free(i);
	}
}

int list_length(struct head_indices* indices){
	int ret = 0;
	struct element_indices* i;
	STAILQ_FOREACH(i, indices, link){
		ret++;
	}
	return ret;
}

/*
 * rb_tree (Rules)
 *     <List (Alternatives)
 *         <List (Sub-Rules)
 *             <String (Literal) or a RULE (searchable in the tree)>
 *         >
 *     >
 *
 * validate_string:
 *     go over RULES{
 *         go over alternatives{
 *             check if string matches Sub-Rule-Chain (on first missmatch -> next alternative)
 *             check if strign is completely consumed -> yes -> break all
 *         }
 *     }
*/

/*
 * TODO New:
 *     - work on indices instead of strings (parameter)
 *     - Return somehow the set (list) of possible next indices (see https://github.com/mebeim/aoc/blob/master/2020/solutions/day19.py)
*/

struct head_indices* validate_rule(struct element_rules* rule, char* string, int rec_lvl, int index){

#ifdef DEBUG
	printf("%*s%d for string: \"%s\"\n", rec_lvl + strlen("Try rule "), "Try rule ", rule->rule_num, string);
#endif


	// empty list with all indices that are being mathced by that rule
	struct head_indices* ret_indices = malloc(sizeof(struct head_indices));
	STAILQ_INIT(ret_indices);

	if(index >= strlen(string)){
#ifdef DEBUG
		printf("%*sIndex at end of string\n", rec_lvl, "");
#endif
		return ret_indices;
	}

	if(rule->alternatives->stqh_first->sub->stqh_first->literal != '\0'){
		char literal = rule->alternatives->stqh_first->sub->stqh_first->literal;
		// rule is just a literal
#ifdef DEBUG
		printf("rule: %d literal '%c'\n", rule->rule_num, literal);
#endif
		if (string[index] == literal){
			// insert the current index + 1 into the returned list
			assert(STAILQ_EMPTY(ret_indices) == 1 && "ret_indices should have been empty");
			struct element_indices* i1 = malloc(sizeof(struct element_indices));
			i1->index = index+1; // take the given index
			STAILQ_INSERT_TAIL(ret_indices, i1, link);
			assert(list_length(ret_indices) == 1 && "ret_indices length should have been 1");
			return ret_indices;
		} else {
			assert(list_length(ret_indices) == 0 && "ret_indices length should have been 0");
			return ret_indices; // empty
		}
	}

	// list in which the possible lengths/indices are stored
	struct head_indices* indices = malloc(sizeof(struct head_indices));
	STAILQ_INIT(indices);

	// temp list in which the next possible indices are stored
	struct head_indices* next_indices = malloc(sizeof(struct head_indices));
	STAILQ_INIT(next_indices);

	// go over all alternatives of the rule
	struct element_alt* alternative;
	STAILQ_FOREACH(alternative, rule->alternatives, link){
#ifdef DEBUG
		printf("%*s\n", rec_lvl + strlen("--------------------------Alternative"), "--------------------------Alternative");
#endif
		assert(STAILQ_EMPTY(indices) == 1 && "indices should have been empty");
		struct element_indices* i1 = malloc(sizeof(struct element_indices));
		i1->index = index; // take the given index
		STAILQ_INSERT_TAIL(indices, i1, link);

		// go over the subrules of the alternative
		struct subrules_ele* subrule;
		STAILQ_FOREACH(subrule, alternative->sub, link){

#ifdef DEBUG
					printf("%*sIndices to be tested: ", rec_lvl, "");
					struct element_indices* muell;
					STAILQ_FOREACH(muell, indices, link){
						printf("%d ", muell->index);
					}
					printf("\n");
#endif
			// go over possible start indices at this point
			struct element_indices* idx;
			STAILQ_FOREACH(idx, indices, link){

#ifdef DEBUG
					printf("%*s\n", rec_lvl + strlen("Subrule-------------------"), "Subrule-------------------");
#endif
					struct head_indices* ret = validate_rule(subrule->next, string, rec_lvl+8, idx->index);
				
#ifdef DEBUG
					printf("%*sReturned indices: ", rec_lvl, "");
					struct element_indices* muell;
					STAILQ_FOREACH(muell, ret, link){
						printf("%d ", muell->index);
					}
					printf("\n");
#endif

					if(!STAILQ_EMPTY(ret)){
						// sub-rule matched at least one time
						STAILQ_CONCAT(next_indices, ret); // add the returned indices to the list of indices to get checked
					}
#ifdef DEBUG
					printf("%*s\n", rec_lvl + strlen("Subrule end-------------------"), "Subrule end-------------------");
#endif
					free(ret);
			} // end go over indices in subrule
			free_list(indices);
			STAILQ_CONCAT(indices, next_indices); // add the matched indices
		} // end go over subrules
		// add the indices that match the current alternative to the returned indices
		// and empty the indices for the next alternative
		STAILQ_CONCAT(ret_indices, indices);
	} // end go over alternatives

	// shoule both be empty
	free(indices);
	free(next_indices);

	//return the index list
	return ret_indices;
}

int part1(struct string_content* input, int start, struct rules_head* rules){
	int counter = 0;
	for(int i = start; i < input->length; i++){
		/* printf("%s\n", input->content[i]); */

		// go over all rules
		struct element_rules* x;
		RB_FOREACH(x, rules_head, rules){
#ifdef DEBUG
			printf("\nRule %d\n", x->rule_num);
#endif
			struct head_indices* ret = validate_rule(x, input->content[i], 0, 0);

			int matched = 0;
			struct element_indices* f;
			struct element_indices* f_save;
			STAILQ_FOREACH_SAFE(f, ret, link, f_save){
				if(!matched && f->index == strlen(input->content[i])){
					printf("%s\n", input->content[i]);
					counter++;
					matched = 1;
				}
				STAILQ_REMOVE(ret, f, element_indices, link);
				free(f);
			}
			free(ret);
			if(matched) break; // don't test other rules as well
		}
	}
	return counter;
}

void print_rules(struct rules_head* rules){
	struct element_rules* x;
	RB_FOREACH(x, rules_head, rules){
		printf("%d(%p):", x->rule_num, x);
		struct element_alt* y;
		STAILQ_FOREACH(y, x->alternatives, link){
			struct subrules_ele* z;
			STAILQ_FOREACH(z, y->sub, link){
				if(z->literal != '\0'){
					printf(" \"%c\"", z->literal);
				} else {
					printf(" %d(%p)", z->next_i, z->next);
				}
			}
			if(STAILQ_NEXT(y, link)){
				printf(" |");
			}
		}
		printf("\n");
	}
}

void read_rules(struct string_content* input, struct rules_head* rules, int* rules_length){
	int i;
	for(i=0; i < input->length && input->content[i][0] != '\0' ; i++){
		char* line = input->content[i];
#ifdef DEBUG_EXT
		printf("%s\n", line);
#endif
		int num = -1;
		int read_bytes = 0;
		sscanf(line, "%d: %n", &num, &read_bytes);
		line += read_bytes;
#ifdef DEBUG_EXT
		printf("%d chars read   left: \"%s\"\n", read_bytes, line);
#endif

		// create rule, initialize it and insert into rules tree
		struct element_rules* rule = malloc(sizeof(struct element_rules));
		struct alt_head* alt_h = malloc(sizeof(struct alt_head));
		STAILQ_INIT(alt_h);
		*rule = (struct element_rules) {.rule_num = num, .alternatives = alt_h};
		RB_INSERT(rules_head, rules, rule);

		// go over alternatives
		while(1){
			// create alternative, initialize it and insert into alt_h list
			struct element_alt* alt = malloc(sizeof(struct element_alt));
			struct subrules_head* subrule_h = malloc(sizeof(struct subrules_head));
			STAILQ_INIT(subrule_h);
			alt->sub = subrule_h;
			STAILQ_INSERT_TAIL(alt_h, alt, link);

			// go over sub rules
			while(1){
				char literal = '\0';
				int  sub     = 0;
				if(sscanf(line, "\"%c\" %n", &literal, &read_bytes) > 0){
					// literal was read
					line += read_bytes;
#ifdef DEBUG_EXT
					printf("%d chars read literal: '%c' left: \"%s\"\n", read_bytes, literal, line);
#endif
				} else if (sscanf(line, "%d %n", &sub, &read_bytes) > 0){
					// subrule was read
					line += read_bytes;
#ifdef DEBUG_EXT
					printf("%d chars read subrule: %d left: \"%s\"\n", read_bytes, sub, line);
#endif
				} else {
#ifdef DEBUG_EXT
					printf("line: \"%s\"\n", line);
#endif
					break; // subrule over
				}
				// create subrule initialze it and insert it into subrule_h list
				struct subrules_ele* subrule = malloc(sizeof(struct subrules_ele));
				*subrule = (struct subrules_ele) {.literal = literal, .next_i = sub, .next = NULL};
				STAILQ_INSERT_TAIL(subrule_h, subrule, link);
			}
			if(*line != '|'){
#ifdef DEBUG_EXT
				printf("Rule if over; line: \"%s\"\n", line);
#endif
				break; // rule is over
			} else {
				line += 2;
#ifdef DEBUG_EXT
				printf("New subrule\n");
				printf("line: \"%s\"\n\n", line);
#endif
			}
		}
	}

	// store the length of the header/rules
	*rules_length = i+1; // skip blank line

	struct element_rules* x;
	RB_FOREACH(x, rules_head, rules){
		printf("%d(%p):", x->rule_num, x);
		struct element_alt* y;
		STAILQ_FOREACH(y, x->alternatives, link){
			struct subrules_ele* z;
			STAILQ_FOREACH(z, y->sub, link){
				if(z->literal != '\0'){
					printf(" \"%c\"", z->literal);
				} else {
					printf(" %d", z->next_i);
					struct element_rules find = {.rule_num = z->next_i};
					z->next = RB_FIND(rules_head, rules, &find);
					printf("(%p)", z->next);
				}
			}
			if(STAILQ_NEXT(y, link)){
				printf(" |");
			}
		}
		printf("\n");
	}

}

void free_rules(struct rules_head* rules){
	struct element_rules* x;
	struct element_rules* x_save;
	RB_FOREACH_SAFE(x, rules_head, rules, x_save){
		struct element_alt* y;
		struct element_alt* y_save;
		STAILQ_FOREACH_SAFE(y, x->alternatives, link, y_save){
			struct subrules_ele* z;
			struct subrules_ele* z_save;
			STAILQ_FOREACH_SAFE(z, y->sub, link, z_save){
				STAILQ_REMOVE(y->sub, z, subrules_ele, link);
				free(z);
			}
			free(y->sub);
			STAILQ_REMOVE(x->alternatives, y, element_alt, link);
			free(y);
		}
		free(x->alternatives);
		RB_REMOVE(rules_head, rules, x);
		free(x);
	}
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-19.dat", type);
	struct rules_head rules;
	RB_INIT(&rules);

	int rules_length = -1;

	read_rules(input, &rules, &rules_length);
	printf("\n");

	/* printf("Part1:\n%d\n", part1(input, rules_length, &rules)); */

	/* return 0; */

		// alter rule 8
	struct element_rules find = {.rule_num = 8};
	struct element_rules* res = RB_FIND(rules_head, &rules, &find);
	
	// insert a new alternative
	struct element_alt* new_alt = malloc(sizeof(struct element_alt));
	STAILQ_INSERT_TAIL(res->alternatives, new_alt, link);

	// create a new sublist
	struct subrules_head* new_sub_h = malloc(sizeof(struct subrules_head));
	STAILQ_INIT(new_sub_h);
	new_alt->sub = new_sub_h;

	struct subrules_ele* scr;
	struct subrules_ele* new_sub;
	// insert the elements
	new_sub = malloc(sizeof(struct subrules_ele));
	scr = res->alternatives->stqh_first->sub->stqh_first; // 42
	*new_sub = (struct subrules_ele) {.literal = scr->literal, .next_i = scr->next_i, .next = scr->next};
	STAILQ_INSERT_TAIL(new_sub_h, new_sub, link);

	new_sub = malloc(sizeof(struct subrules_ele));
	*new_sub = (struct subrules_ele) {.literal = '\0', .next_i = 8, .next = res};
	STAILQ_INSERT_TAIL(new_sub_h, new_sub, link);

	// alter rule 11
	find.rule_num = 11;
	res = RB_FIND(rules_head, &rules, &find);
	
	// insert a new alternative
	new_alt = malloc(sizeof(struct element_alt));
	STAILQ_INSERT_TAIL(res->alternatives, new_alt, link);

	// create a new sublist
	new_sub_h = malloc(sizeof(struct subrules_head));
	STAILQ_INIT(new_sub_h);
	new_alt->sub = new_sub_h;

	// insert the elements
	new_sub = malloc(sizeof(struct subrules_ele));
	scr = res->alternatives->stqh_first->sub->stqh_first; // 42
	*new_sub = (struct subrules_ele) {.literal = scr->literal, .next_i = scr->next_i, .next = scr->next};
	STAILQ_INSERT_TAIL(new_sub_h, new_sub, link);

	new_sub = malloc(sizeof(struct subrules_ele));
	*new_sub = (struct subrules_ele) {.literal = '\0', .next_i = 11, .next = res};
	STAILQ_INSERT_TAIL(new_sub_h, new_sub, link);

	new_sub = malloc(sizeof(struct subrules_ele));
	scr = res->alternatives->stqh_first->sub->stqh_first->link.stqe_next; // 31
	*new_sub = (struct subrules_ele) {.literal = scr->literal, .next_i = scr->next_i, .next = scr->next};
	STAILQ_INSERT_TAIL(new_sub_h, new_sub, link);

	print_rules(&rules);

	printf("\nPart2:\n%d\n", part1(input, rules_length, &rules));

	free_rules(&rules);

	free_file(input);
	return EXIT_SUCCESS;
}
