#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./read_file.h"

#include <bsd/sys/tree.h>

struct element_tree {
	char* field_name; // key
	int min1;
	int max1;
	int min2;
	int max2;
	RB_ENTRY(element_tree) link;
};
int cmp(struct element_tree *a, struct element_tree *b){
	return strcmp(a->field_name, b->field_name);
}
RB_HEAD(ticket_fields, element_tree);
RB_PROTOTYPE(ticket_fields, element_tree, link, cmp)
RB_GENERATE(ticket_fields, element_tree, link, cmp)

void parse_fields(struct string_content* input, int amount_fields, struct ticket_fields* fields){
	for(int i=0; i < amount_fields; i++){
		char* rest = NULL;
		char* name = strtok_r(input->content[i], ":", &rest);

		int min1 = 0, max1 = 0, min2 = 0, max2 = 0;
		sscanf(rest+1, "%d-%d or %d-%d", &min1, &max1, &min2, &max2);

		/* printf("Name: %s: first: %d - %d; sec: %d - %d\n", name, min1, max1, min2, max2); */

		struct element_tree* x = malloc(sizeof(struct element_tree));
		*x = (struct element_tree) {.field_name = name, .min1 = min1, .max1 = max1, .min2 = min2, .max2 = max2};
		RB_INSERT(ticket_fields, fields, x);
	}
}

int part1(struct string_content* input, int header_length, int*** tickets, struct ticket_fields* ticket_fields){
	RB_INIT(ticket_fields);

	parse_fields(input, header_length, ticket_fields);

	/* printf("Malloced %d\n\n", input->length-header_length-4); */
	(*tickets) = malloc(sizeof(int*)*input->length-header_length-4);
	int acc = 0;

	// go over the tickets
	for(int i=header_length+5; i < input->length; i++){
		(*tickets)[i-header_length-5] = malloc(sizeof(int)*header_length);
		/* printf("Malloced %d\n", header_length); */

		char* elem = strtok(input->content[i], ",");
		int num = -1;
		// go over the ticket fields
		for(int j=0; elem != NULL; j++){
			num = atoi(elem);

			/* printf("Accesss %d %d\n", i-header_length-5, j); */
			/* printf("Addr: %x\n", tickets); */
			/* printf("Addr: %x\n", (*tickets)[i-header_length-5]); */
			(*tickets)[i-header_length-5][j] = num;

			int valid_num = 0;
			struct element_tree* x;
			// check if number matches any field constraints
			RB_FOREACH(x, ticket_fields, ticket_fields){
				// as soon as it fits into one field the number is valid
				if((num >= x->min1 && num <= x->max1) || (num >= x->min2 && num <= x->max2)){
					/* printf("Number %d is valid\n", num); */
					valid_num = 1;
					break;
				}
			}

			// check if number is invalid
			if(!valid_num){
				free((*tickets)[i-header_length-5]);
				(*tickets)[i-header_length-5] = NULL;
				acc += num;
				break;
			}

			elem = strtok(NULL, ",");
		}

	}

	// add the own ticket to the array
	(*tickets)[input->length-header_length-5] = malloc(sizeof(int)*header_length);
	char* elem = strtok(input->content[header_length+2], ",");
	int num = -1;
	// go over the ticket fields
	for(int j=0; elem != NULL; j++){
		/* printf("%s\nAccess: %d %d\n", elem, input->length-header_length-5, j); */
		num = atoi(elem);
		(*tickets)[input->length-header_length-5][j] = num;
		elem = strtok(NULL, ",");
	}

	return acc;
}

unsigned long part2(struct string_content* input, int header_length, int** tickets, struct ticket_fields* ticket_fields){
	// arr what is possible (0/1) loop over ticket_fields and set to 0 if field is not valid
	int **poss = malloc(sizeof(int*)*header_length); // TODO annotate what index is what
	for(int i=0; i < header_length; i++){
		poss[i] = malloc(sizeof(int)*header_length);
		for(int j=0; j < header_length; j++){
			poss[i][j] = 1;
		}
	}

	// go over tickets
	for(int i=0; i < input->length-header_length-4; i++){
		if(tickets[i] == NULL){
			/* printf("ticket invalid\n"); */
			continue;
		}
		// go over the ticket fields
		for(int j=0; j < header_length; j++){
			/* printf("Checking: %d\n", tickets[i][j]); */
			// go over the possible fields
			struct element_tree* x;
			int fp = 0;
			RB_FOREACH(x, ticket_fields, ticket_fields){
				/* if(!poss[j][fp]) continue; */

				if(!((tickets[i][j] >= x->min1 && tickets[i][j] <= x->max1) || (tickets[i][j] >= x->min2 && tickets[i][j] <= x->max2))){
					// if field is invalid
					poss[j][fp] = 0;
					/* printf("Field %d (%s) cannot be at position %d\n", fp, x->field_name, j); */
				}
				fp++;
			}
		}
		/* printf("\n"); */
	}

	/* for(int i=0; i < header_length; i++){ */
	/* 	printf("%02d | ", i); */
	/* 	for(int j=0; j < header_length; j++){ */
	/* 		printf("%d ", poss[i][j]); */
	/* 	} */
	/* 	printf("\n"); */
	/* } */

	// TODO as long as nothing changes
	for(int z=0; z < 10; z++){
		for(int i=0; i < header_length; i++){
			int count = 0;
			int col = -1;
			for(int j=0; j < header_length; j++){
				count += poss[i][j];
				if(poss[i][j] != 0) col=j;
			}
			if(count <= 1){
				for(int j=0; j < header_length; j++){
					if(j != i) poss[j][col] = 0;
				}
			}
		}
	}
	
	/* printf("\n"); */

	/* for(int i=0; i < header_length; i++){ */
	/* 	printf("%02d | ", i); */
	/* 	for(int j=0; j < header_length; j++){ */
	/* 		printf("%d ", poss[i][j]); */
	/* 	} */
	/* 	printf("\n"); */
	/* } */

	struct element_tree* x;
	unsigned long acc = 1;
	int fp = 0;
	RB_FOREACH(x, ticket_fields, ticket_fields){
		int place = -1;
		// check at what place the field is (TODO simple lookup somehow possible?)
		for(int j=0; j < header_length; j++){
			if(poss[j][fp]){
				place = j;
				break;
			}
		}
		if(!strncmp(x->field_name, "departure", 8)){
			/* printf("Field: %s -> %d\n", x->field_name, tickets[input->length-header_length-5][place]); */
			acc *= tickets[input->length-header_length-5][place];
		}
		fp++;
	}

	// free everything
	// TODO maybe free some things earlier
	for(int i=0; i < input->length-header_length-4; i++){
		free(tickets[i]);
	}
	free(tickets);

	for(int i=0; i < header_length; i++){
		free(poss[i]);
	}
	free(poss);

	struct element_tree *y;
	RB_FOREACH_SAFE(x, ticket_fields, ticket_fields, y){
		RB_REMOVE(ticket_fields, ticket_fields, x);
		free(x);
	}

	return acc;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-16.dat", type);
	int** tickets;
	struct ticket_fields ticket_fields;
	printf("Part1:\nAmount of wrong read tickets: %d\n", part1(input, 20, &tickets, &ticket_fields));
	printf("\nPart2:\nProduct of the Values of the Fields starting with \"depature\" on own ticket: %lu\n", part2(input, 20, tickets, &ticket_fields));
	free_file(input);
	return EXIT_SUCCESS;
}
