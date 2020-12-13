#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "./read_file.h"

// TODO inline
int kl_v(int a, int b, int ggt){
	return (a*b)/ggt;
}

int ea(int a, int b){
	assert(0 <= a && a <= b && "precondition in ea not met");
	if (a == 0){
		return b;
	}
	return ea(b % a, a);
}

int ea_start(int a, int b){
	if(a <= b){
		return ea(a,b);
	}
	return ea(b,a);
}

void eea(int a, int b, int ret[2]){
	assert(0 <= a && a <= b && "precondition in eea not met");
	if(a == 0){
		ret[0] = 0; ret[1] = 1;
		return;
	}
	int k = b / a;
	int r = b % a;
	if(r == 0){
		ret[0] = 1; ret[1] = 0;
		return;
	}

	int param[2] = {0};
	eea(r, a, param);
	ret[0] = param[1]-k*param[0];
	ret[1] = param[0];
	return;
}

void eea_start(int a, int b, int ret[2]){
	if(a <= b){
		eea(a,b, ret);
	} else {
		eea(b,a, ret);
	}
}

int part1(struct string_content* input){
	int dep_time = atoi(input->content[0]);

	int min = INT_MAX;
	int line = -1;
	char* num = strtok(input->content[1], ",");
	while(num != NULL){
		if(*num == 'x'){num = strtok(NULL, ","); continue; }
		int period = atoi(num);
		/* printf("%s\t%d\t%d\n", num, dep_time/period, dep_time%period); */

		int next = period*((dep_time/period)+1);
		if(next < min){
			min = next;
			line = period;
			/* printf("New minimum: %d with line %d\n", min, line); */
		}
		/* printf("\n"); */
		num = strtok(NULL, ",");
	}
	return (min-dep_time)*line;
}

int part2(struct string_content* input, int amount){
	int* arr    = malloc(sizeof(int)*amount);
	int* offset = malloc(sizeof(int)*amount);
	char* num = strtok(input->content[1], ",");
	int i = 0;
	for(int j = 0; num != NULL && i < amount; j++){
		if(*num != 'x'){
			offset[i] = j;
			arr[i++]  = atoi(num);
		}
		num = strtok(NULL, ",");
	}

	for(int i=0; i < amount; i++){
		printf("%d\t", offset[i]);
		printf("%d\n", arr[i]);
	}

	int zero = arr[0];
	int all_hit = 0;
	for(int i=1; i < amount; i++){
		int ggt = ea_start(zero,arr[i]);
		int alpha_beta[2] = {0};
		eea_start(zero, arr[i], alpha_beta);
		int kgv = kl_v(zero, arr[i], ggt);

		int hit = (((offset[i]/ggt)*alpha_beta[0]*zero) % kgv + kgv) % kgv;
		printf("%-3d and %-3d hit at %-5d with offset %d\n", arr[i], zero, hit, offset[i]);
		if(all_hit != 0){
			all_hit = kl_v(all_hit, hit, ea_start(hit,all_hit));
		} else {
			all_hit = hit;
		}
	}
	return all_hit;
}

int main() {
	/* printf("%d\n", ea(5,911)); */
	/* printf("%d %d\n", in[0], in[1]); */
	/* eea(45,63,in); */
	/* printf("%d\n", ea(45,63)); */
	/* printf("%d %d\n", in[0], in[1]); */
	/* return 0; */


	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./day-13.dat", type);
	printf("Part1:\n%d\n", part1(input));
	free_file(input);
	
	input = read_file("./day-13.dat.testing2", type);
	printf("\nPart2:\n%d\n", part2(input,5));
	free_file(input);
	return EXIT_SUCCESS;
}
