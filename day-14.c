#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <bsd/sys/tree.h>

#include "./read_file.h"

struct element_tree {
	unsigned long addr;
	long val;
	RB_ENTRY(element_tree) link;
};
int cmp(struct element_tree *a, struct element_tree *b){
	/* printf("a: %d, b: %d\n", a->addr, b->addr); */
	return (a->addr < b->addr ? -1 : a->addr > b->addr);
}
RB_HEAD(mem, element_tree);
RB_PROTOTYPE(mem, element_tree, link, cmp)
RB_GENERATE(mem, element_tree, link, cmp)

char* bitstring(unsigned long input, char output[sizeof(long)*8]){
	int i = sizeof(long)*8 - 1;
	while(input > 0){
		if((input % 2) == 0){
			output[i--] = '0';
		} else {
			output[i--] = '1';
		}
		input = input / 2;
	}
	while(i >= 0){
		output[i--] = '0';
	}
	return output;
}

char* replace(char* in, char find, char subs){
	char* save = in;
	while(*in){
		if(*in == find) *in = subs;
		in++;
	}
	return save;
}

char* replace2(char* in, char find1, char subs1, char find2, char subs2){
	char* save = in;
	while(*in){
		if(*in == find1) *in = subs1;
		else if(*in == find2) *in = subs2;
		in++;
	}
	return save;
}

unsigned long part1(struct string_content* input){
	struct mem* memory = malloc(sizeof(struct mem));
	RB_INIT(memory);

	for(int i=0; i < input->length; i++){
		// process new mask
		char* mask = NULL;
		sscanf(input->content[i], "mask = %ms", &mask);
		
		// split mask into set and clear bits and which bits to leave through
		// read it as number and free the memory
		char* leave_mask_through_s = replace2(strdup(mask), '1', '0', 'X', '1');
		char* mask_set_s           = replace(strdup(mask), 'X', '0');
		char* mask_clear_s         = replace(strdup(mask), 'X', '1');

		unsigned long leave_mask_through = strtol(leave_mask_through_s, NULL, 2);
		unsigned long mask_set   = strtol(mask_set_s, NULL, 2);
		unsigned long mask_clear = strtol(mask_clear_s, NULL, 2);

		free(mask);
		free(leave_mask_through_s);
		free(mask_clear_s);
		free(mask_set_s);

		i++; // go to instructions to operate on that mask
		while(i < input->length){
			unsigned long val = 0;
			int ind = 0;
			if (sscanf(input->content[i], "mem[%d] = %lu\n", &ind, &val) < 2){
				// if getting the next instruction fails, go one step back and try to parse the mask
				i--;
				break;
			}

			// search if the memory cell aleady exists
			struct element_tree find = {.addr = ind};
			struct element_tree* res = RB_FIND(mem, memory, &find);
			if(res == NULL){
				// if it is a new cell, create a new entry
				res = malloc(sizeof(struct element_tree));
				res->addr = ind;
				res->val = 0;
				mem_RB_INSERT(memory, res);
			}

			// set the value and apply the masks on it
			res->val = val;
			res->val = res->val & leave_mask_through;
			res->val = res->val | mask_set;
			res->val = res->val & mask_clear;

			/* printf("Value at %d : %ld\n", ind, res->val); */

			i++;
		}
	}

	// iterate over the memory and sum up the values
	struct element_tree* i;
	struct element_tree* j;
	unsigned long acc = 0;
	RB_FOREACH_SAFE(i, mem, memory, j){
		acc += i->val;
		RB_REMOVE(mem, memory, i);
		free(i);
	}
	free(memory);
	return acc;
}

/**
 * permutate the floating bits and set the values in the memory
 * @param acc the accumulatet floating bitmask for the address
 * @param indmask which bits are floating
 * @param index which bit is currently processed
 * @param max_ind how many indices are available
 * @param start_index line number to be processed
 * @param memory pointer to the tree representing the memory
 * @param input struct containing the input file
 * @param clear_x mask which clears the nessacary bits
 * @param base_set mask which srts the nessacary bits
 */
void rec_perm(unsigned long acc, unsigned long indmask, int index, int max_ind, int start_index, struct mem* memory, struct string_content* input, unsigned long clear_x, unsigned long base_set, int* x){
	// check if all bits are processed
	if(index >= max_ind){
		int i = start_index+1; // go to instructions to operate on that mask
		while(i < input->length){
			unsigned long val = 0;
			unsigned long ind = 0;
			if (sscanf(input->content[i], "mem[%lu] = %lu\n", &ind, &val) < 2){
				*x = i;
				break;
			}

			ind = ind & clear_x;
			ind = ind | base_set;
			ind = ind | acc;

			// find element with addr ind (create otherwise)
			struct element_tree find = {.addr = ind};
			struct element_tree* res = RB_FIND(mem, memory, &find);
			if(res == NULL){
				res = malloc(sizeof(struct element_tree));
				if (res == NULL) exit(-1);
				res->addr = ind;
				mem_RB_INSERT(memory, res);
			}
			res->val = val;

			/* printf("Value at %lu is %ld ind: %lu\n", res->addr, res->val, ind); */

			i++;
		}
		// update the linepointer of the mask loop
		*x = i;
		return;
	}

	unsigned long indmask_set = indmask & ((unsigned long)1 << (unsigned long)index);

	// split only into two if the current bit is really floating
	if(indmask_set){
		/* printf("at index %d the bit is set!\n", index); */
		indmask_set = acc | ((unsigned long)1 << (unsigned long)index);
		// recursive call with current bit set
		rec_perm(indmask_set, indmask, index+1, max_ind, start_index, memory, input, clear_x,base_set, x);
	}
	// recursive call with current bit cleared
	rec_perm(acc, indmask, index+1, max_ind, start_index, memory, input, clear_x, base_set, x);
}

unsigned long part2(struct string_content* input){
	struct mem* memory = malloc(sizeof(struct mem));
	RB_INIT(memory);

	for(int i=0; i < input->length; i++){
		// read mask
		char* mask = NULL;
		sscanf(input->content[i], "mask = %ms", &mask);

		// create mask with floating and basis bits and convert them to integers
		char* floating_set_s = replace2(strdup(mask), '1', '0', 'X', '1');
		char* base_set_s     = replace(strdup(mask), 'X', '0');

		unsigned long floating_set = strtol(floating_set_s, NULL, 2);
		unsigned long base_set     = strtol(base_set_s, NULL, 2);

		free(mask);
		free(base_set_s);
		free(floating_set_s);

		// iterate recursively over the possible permutations of that floating mask
		rec_perm(0, floating_set, 0, 36, i, memory, input, ~floating_set, base_set, &i);

		// i points to the line with the mask but due to the for loop it will be increased
		i--;
	}

	// sum up the values of the memory
	struct element_tree* i;
	struct element_tree* j;
	unsigned long acc = 0;
	RB_FOREACH_SAFE(i, mem, memory, j){
		acc += (unsigned long)i->val;
		RB_REMOVE(mem, memory, i);
		free(i);
	}
	free(memory);

	return acc;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-14.dat", type);
	printf("Part1:\nSummed values in memory: %lu\n", part1(input));
	free_file(input);
	
	input = read_file("./day-14.dat", type);
	printf("\nPart2:\nSummed values in memory: %lu\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
