#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int validate_rule(struct element_rules* rule, char* string, int* amount_matched){

#ifdef DEBUG
	printf("Try rule %d\n", rule->rule_num);
#endif

	char* string_save = string; // all alternatives start at the same string
	// go over all alternatives of the rule
	struct element_alt* alternative;
	STAILQ_FOREACH(alternative, rule->alternatives, link){

		int matches = 1; // store if the alternative has been matched
		int counter_in = 0;

		// go over the subrules of the alternative
		struct subrules_ele* subrule;
		STAILQ_FOREACH(subrule, alternative->sub, link){
			if(subrule->literal != '\0'){
				// subrule is a literal
				if (*string == subrule->literal){
					string++;
					counter_in++;
				} else {
					matches = 0; // alternative does not match the string
					break;
				}
			} else {
				// subrule is another rule
				int add = 0;
				
#ifdef DEBUG
				printf("Subrule-------------------\n");
#endif
				if(validate_rule(subrule->next, string, &add)){
#ifdef DEBUG
					printf("Subrule end-------------------\n");
#endif
					string += add; // skip as many chars as were matched
					counter_in += add;
				} else {
#ifdef DEBUG
					printf("Subrule end-------------------\n");
#endif
					matches = 0; // alternative does not match the string
					break;
				}
			}
		}
		if(matches){
			*amount_matched = counter_in;
			return 1; // one alternative is able to match chars
		}
		string = string_save; // if alternative didn't match restore string position
	}

	return 0;
}

int part1(struct string_content* input, int start, struct rules_head* rules){
	int counter = 0;
	for(int i = start; i < input->length; i++){
		/* printf("%s\n", input->content[i]); */

		// go over all rules
		struct element_rules* x;
		RB_FOREACH(x, rules_head, rules){
			int matched;
#ifdef DEBUG
			printf("\nRule %d\n", x->rule_num);
#endif
			int ret = validate_rule(x, input->content[i], &matched);
#ifdef DEBUG
			printf("=> %d\n",ret);
#endif
			if(ret){
				if(matched == strlen(input->content[i])){
					printf("\"%s\"-------------------MATCHES------------------------------------\n", input->content[i]);
					counter++;
					break;
				}
			}
		}
	}
	return counter;
}

int part2(struct string_content* input){
	return 0;
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
	input = read_file("./day-19.dat.testing2", type);
	struct rules_head rules;
	RB_INIT(&rules);

	int rules_length = -1;

	read_rules(input, &rules, &rules_length);
	printf("\n");

	printf("Part1:\n%d\n", part1(input, rules_length, &rules));

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
