#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "./read_file.h"

int compare(const void * a, const void * b){
	return *(int*)b - *(int*)a;
}

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
		int tmp = ret[1];
		ret[1] = ret[0];
		ret[0] = tmp;
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

	int last = arr[0];
	int l_offset = 0;
	for(int i=1; i < amount; i++){
		printf("Processing %d\n", arr[i]);
		int ggt = ea_start(last,arr[i]);
		int alpha_beta[2] = {0};
		eea_start(last, arr[i], alpha_beta);
		int kgv = kl_v(last, arr[i], ggt);

		printf("ggt: %d  eea: %d %d   kgv: %d\n", ggt, alpha_beta[0], alpha_beta[1], kgv);

		int hit;
		if(last < arr[i]){
			hit = (((l_offset-offset[i]/ggt)*alpha_beta[0]*last - l_offset) % kgv + kgv) % kgv;
		} else {
			hit = (((l_offset-offset[i]/ggt)*alpha_beta[0]*last - l_offset) % kgv + kgv) % kgv;
		}
		printf("%-3d and %-3d hit at %-5d with offset %d\n\n", arr[i], last, hit, offset[i]);

		/* l_offset = hit; */
		last = kgv;
	}
	return last;
}

int search(int a, int* b){
	int i = 0;
	while(b[i] != a) i++;
	return i;
}

long part2b(struct string_content* input, int amount){
	int* arr    = malloc(sizeof(int)*amount);
	int* arr_s  = malloc(sizeof(int)*amount);
	int* offset = malloc(sizeof(int)*amount);
	int* mods   = malloc(sizeof(int)*amount);
	char* num = strtok(input->content[1], ",");
	int i = 0;
	for(int j = 0; num != NULL && i < amount; j++){
		if(*num != 'x'){
			offset[i] = j;
			arr[i]    = atoi(num);
			arr_s[i]  = arr[i];
			mods[i]   = (-j % arr[i] + arr[i]) % arr[i];
			i++;
		}
		num = strtok(NULL, ",");
	}

#ifdef DEBUG
	printf("%-5s\t%-5s\t%-5s\n", "offset", "mod", "line");
	for(int i=0; i < amount; i++){
		printf("%-5d\t", offset[i]);
		printf("%-5d\t", mods[i]);
		printf("%-5d\n", arr[i]);
	}
#endif
	qsort(arr_s, amount, sizeof(int), compare);
	long r = arr_s[0];
	long v = mods[search(arr_s[0], arr)];
#ifdef DEBUG
	printf("r: %ld\t v: %ld\n", r, v);
#endif

	for(int i=1; i < amount; i++){
		long b = arr_s[i];
#ifdef DEBUG
		printf("b: %ld\n", b);
		printf("mods[b]: %d (search for b in arr)\n", mods[search(b, arr)]);
#endif
		while(v % b != mods[search(b, arr)]){
			v += r;
		}
		r *= b;
	}
	return v;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./problems/day-13.dat", type);
	printf("Part1:\n%d\n", part1(input));
	free_file(input);
	
	input = read_file("./problems/day-13.dat", type);
	printf("\nPart2:\n%ld\n", part2b(input,9));
	free_file(input);
	return EXIT_SUCCESS;
}
