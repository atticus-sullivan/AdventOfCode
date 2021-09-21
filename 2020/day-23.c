#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "./read_file.h"

int get_index(int* clock, int len, int search){
	for(int i = 0; i < len; ++i){
		if(clock[i] == search){
			return i;
		}
	}
	return -1;
}

int search_dst(int* clock, int len, int search, int min, int max){
	// calcuate dst:
	int dst = 0;
	while(1){
		dst = get_index(clock, len, search);
		if(dst >= 0){
			break;
		}
		search--;
		if(search < min) search = max;
	}
	return dst;
}

int step(int** clock, int len, int index, int min, int max){
	int ret_idx = 0;
	// src: clock[index+1 % len], clock[index+2 % len], clock[index+3 % len]
	int* clock_new = malloc(len * sizeof(int));
	int* clock_tmp = malloc((len-3) * sizeof(int));

	// copy elements, leave to be moved out
	int j = 0;
	for(int i = 0; i < len; ++i){
		if(i != (index+3) % len && i != (index+1) % len && i != (index+2) % len)
			clock_tmp[j++] = (*clock)[i];
	}

	// copy elemnts insert moved elements again
	int dst = search_dst(clock_tmp, len-3, (*clock)[index]-1, min, max);
	/* printf("dst: %d index: %d\n", dst, index); */

	int idx = index;
	// adjust index to fit to the clock_tmp
	if(idx+3 >= len){
		// idx-(l-idx+4)
		idx = idx - (idx + 4 - len);
	}
	j = 0;
	int j2 = 0;
	for(int i = 0; i < len; ++i){
		if(j > dst && j2 <= dst+3){
			clock_new[i] = (*clock)[(index+j2-dst)% len];
			/* printf("clock_new[%d] = %d ((*clock)[index+j-dst])\n", i, (*clock)[(index+j2-dst) % len]); */
			j2++;
		} else {
			if(j == idx){
				ret_idx = i;
			}
			clock_new[i] = clock_tmp[j];
			/* printf("clock_new[%d] = %d (clock_tmp[j])\n", i, clock_tmp[j]); */
			j++;
			j2++;
		}
	}
	free(*clock);
	free(clock_tmp);
	*clock = clock_new;
	return ret_idx;
}

int part1(struct string_content* input){
	int len = strlen(input->content[0]);
	int *clock = malloc(len * sizeof(int));
	int min = INT_MAX; int max = INT_MIN;
	for(int i = 0; i < len; i++){
		clock[i] = input->content[0][i] - '0';
		if (clock[i] < min)
			min = clock[i];
		if (clock[i] > max)
			max = clock[i];
	}

	int index = 0;
	for(int i = 0; i < 100; ++i){
		/* for(int i=0; i<len; ++i){ */
		/* 	printf("%d ", clock[i]); */
		/* } */
		/* printf("\n"); */

		index = (step(&clock, len, index, min, max) + 1) % len;

		/* for(int i=0; i<len; ++i){ */
		/* 	printf("%d ", clock[i]); */
		/* } */
		/* printf("\n\n"); */
	}
	for(int i=0; i<len; ++i){
		printf("%d ", clock[i]);
	}
	printf("\n\n");

	free(clock);
	return 0;
}

#define size 1000000 // 1 000 000
#define rounds 10000000 // 10 000 000
// #define rounds 1000 // 10 000 000

int part2(struct string_content* input){
	int len = strlen(input->content[0]);
	int *clock = malloc(size * sizeof(int));
	int min = INT_MAX; int max = INT_MIN;
	for(int i = 0; i < len; i++){
		clock[i] = input->content[0][i] - '0';
		if (clock[i] < min)
			min = clock[i];
		if (clock[i] > max)
			max = clock[i];
	}
	for(int i = len; i < size; i++){
		clock[i] = max+i-len+1;
	}
	len = size;

	int index = 0;
	for(int i = 0; i < rounds; ++i){
		if(i % (rounds/1000) == 0)
			printf("Progress: %d of %d\n", i / (rounds/1000), rounds / (rounds/1000));
		index = (step(&clock, len, index, min, max) + 1) % len;
	}
	for(int i=0; i<len; ++i){
		printf("%d ", clock[i]);
	}
	printf("\n\n");

	free(clock);
	return 0;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-23.dat", type);
	printf("Part1:\n%d\n", part1(input));
	free_file(input);

	input = read_file("./day-23.dat", type);
	printf("\nPart2:\n%d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
