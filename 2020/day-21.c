#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "queue.h"

#include "./read_file.h"

struct ing_ele {
  char *ing;
  RB_ENTRY(ing_ele) link;
};
int cmp(struct ing_ele *a, struct ing_ele *b) {
	/* printf("%s\n", a->ing); */
	/* printf("%s\n", b->ing); */
	return strcmp(a->ing, b->ing);
}
RB_HEAD(ingredients, ing_ele);
RB_PROTOTYPE(ingredients, ing_ele, link, cmp)
RB_GENERATE(ingredients, ing_ele, link, cmp)

struct alg_ele {
  char *alergen;
  struct ingredients* possible_ings;
  RB_ENTRY(alg_ele) link;
};
int cmp2(struct alg_ele *a, struct alg_ele *b) { return strcmp(a->alergen, b->alergen); }
RB_HEAD(alergens, alg_ele);
RB_PROTOTYPE(alergens, alg_ele, link, cmp2)
RB_GENERATE(alergens, alg_ele, link, cmp2)

struct queue_ele {
  struct ingredients* ings;
  SLIST_ENTRY(queue_ele) link;
};
SLIST_HEAD(queue, queue_ele);

struct ingredients* ingredients_RB_COPY(struct ingredients* a){
	struct ingredients* ret = malloc(sizeof(struct ingredients));
	RB_INIT(ret);
	struct ing_ele* i = NULL;
	RB_FOREACH(i, ingredients, a){
		struct ing_ele* i_new = malloc(sizeof(struct ing_ele));
		i_new->ing = i->ing;
		ingredients_RB_INSERT(ret, i_new);
	}
	return ret;
}

void ingredients_RB_PRINT(struct ingredients* a);

void ingredients_RB_SETMINUS(struct ingredients* a, struct ingredients* b){
	struct ing_ele* i = NULL;
	struct ing_ele* i_s = NULL;
	RB_FOREACH_SAFE(i, ingredients, a, i_s){
		if(!ingredients_RB_FIND(b, i)){
			ingredients_RB_REMOVE(a, i); // TODO free
		}
	}
}

void ingredients_RB_PRINT(struct ingredients* a){
	struct ing_ele* i = NULL;
	RB_FOREACH(i, ingredients, a){
		printf("> %s\n", i->ing);
	}
}

_Bool alergen_RB_ING_CONTAINED(struct alergens* alg, struct ing_ele* ing){
	struct alg_ele* i = NULL;
	RB_FOREACH(i, alergens, alg){
		if(ingredients_RB_FIND(i->possible_ings, ing)){
			return 1;
		}
	}
	return 0;
}

int ingredients_RB_SIZE(struct ingredients* a){
	int ret = 0;
	struct ing_ele* i = NULL;
	RB_FOREACH(i, ingredients, a){
		ret++;
	}
	return ret;
}

void alergen_RB_REMOVE_AL_ING(struct alergens* alg, struct alg_ele* except, struct ing_ele* ing){
	struct alg_ele* i = NULL;
	RB_FOREACH(i, alergens, alg){
		if(i == except) continue;
		struct ing_ele* rem = ingredients_RB_FIND(i->possible_ings, ing);
		if(rem != NULL){
			/* printf("remove %s from %s\n", rem->ing, i->alergen); */
			ingredients_RB_REMOVE(i->possible_ings, rem); // TODO free
		}
	}
}

int part1(struct string_content* input, struct alergens* algs, struct queue* qu){
	// iterate over lines
	for(int i=0; i < input->length; i++){
		// create ingredients set
		struct ingredients* ingredients = malloc(sizeof(struct ingredients));
		RB_INIT(ingredients);
		// tokenize ingredients
		char* saveprt = NULL;
		char* ing_s = strtok_r(input->content[i], " ", &saveprt);
		// create ingredient
		struct ing_ele *ing = malloc(sizeof(struct ing_ele));
		ing->ing = ing_s;
		ingredients_RB_INSERT(ingredients, ing);
		while(*saveprt != '('){ // parse ingredients
			ing_s = strtok_r(NULL, " ", &saveprt);
			// create ingredient
			struct ing_ele *ing = malloc(sizeof(struct ing_ele));
			ing->ing = ing_s;
			ingredients_RB_INSERT(ingredients, ing);
		}
		// insert ingredients list
		struct queue_ele* qu_ele = malloc(sizeof(struct queue_ele));
		qu_ele->ings = ingredients;
		SLIST_INSERT_HEAD(qu, qu_ele, link);

		strtok_r(NULL, " ", &saveprt); // skip contains

		char* alg_s = NULL;
		// tokenize alergens
		_Bool running = 1;
		while(running){ // parse alergens
			alg_s = strtok_r(NULL, ", ", &saveprt);
			if(*(saveprt-1) == ')'){
				running = 0;
				*(saveprt-1) = '\0';
			}
			struct alg_ele* alg = malloc(sizeof(struct alg_ele));
			alg->alergen = alg_s;
			struct alg_ele* ret = NULL;
			ret = alergens_RB_INSERT(algs, alg);
			if(ret == NULL){
				// successfull inserstion
				alg->possible_ings = ingredients_RB_COPY(ingredients);
			} else {
				// perform setminus
				ingredients_RB_SETMINUS(ret->possible_ings, ingredients);
			}
		}
	}

	int ret = 0;
	struct queue_ele* i1 = NULL;
	SLIST_FOREACH(i1, qu, link){
		struct ing_ele* i2 = NULL;
		RB_FOREACH(i2, ingredients, i1->ings){
			if(!alergen_RB_ING_CONTAINED(algs, i2)){
				ret++;
			}
		}
	}
	return ret;
}


int part2(struct string_content* input, struct alergens* algs, struct queue* qu){
	(void)input;
	(void)qu;
	_Bool running = 1;
	struct alg_ele* i = NULL;

	/* RB_FOREACH(i, alergens, algs){ */
	/* 	printf("%s ->\n", i->alergen); */
	/* 	ingredients_RB_PRINT(i->possible_ings); */
	/* 	printf("\n"); */
	/* } */
	/* printf("\n"); */
	/* printf("\n"); */

	while(running){
		running = 0;
		RB_FOREACH(i, alergens, algs){
			if(ingredients_RB_SIZE(i->possible_ings) == 1){
				/* printf("found %s with %s\n", i->alergen, i->possible_ings->rbh_root->ing); */
				alergen_RB_REMOVE_AL_ING(algs, i, i->possible_ings->rbh_root);
			} else {
				running = 0;
			}
		}
	}

	/* printf("\n"); */
	printf("\nSolution: ");

	RB_FOREACH(i, alergens, algs){
		printf("%s,", i->possible_ings->rbh_root->ing);
	}
	return 0;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	struct alergens* algs = malloc(sizeof(struct alergens));
	RB_INIT(algs);

	struct queue* qu = malloc(sizeof(struct queue));
	SLIST_INIT(qu);

	input = read_file("./day-21.dat", type);
	printf("Part1:\n%d\n", part1(input, algs, qu));
	
	printf("\nPart2:\n%d\n", part2(input, algs, qu));
	free_file(input);
	return EXIT_SUCCESS;
}

// TODO free data structures
