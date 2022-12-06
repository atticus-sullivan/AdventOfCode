#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

#include <bsd/sys/tree.h>

struct tile {
	int x;
	int y;// key
	RB_ENTRY(tile) link;
};

int cmp(struct tile *a, struct tile *b){
	if(a->x < b->x)
		return -1;
	if(a->x > b->x)
		return 1;
	// if both x values are equal compare the y values
	if(a->y < b->y)
		return -1;
	if(a->y > b->y)
		return 1;
	return 0;
}

struct field {
	struct tile *rbh_root; /* root of the tree */
	int xMin;
	int xMax;
	int yMin;
	int yMax;
};

RB_PROTOTYPE(field, tile, link, cmp)
RB_GENERATE(field, tile, link, cmp)

void RB_DESTROY(struct field* field){
	struct tile* i = NULL;
	struct tile* j = NULL;
	RB_FOREACH_SAFE(i, field, field, j){
		field_RB_REMOVE(field, i);
		free(i);
	}
	free(field);
}

struct field* RB_COPY(struct field* field){
	struct field* ret = malloc(sizeof(struct field));
	RB_INIT(ret);
	ret->xMax = field->xMax; ret->xMin = field->xMin; ret->yMax = field->yMax; ret->yMin = field->yMin;
	struct tile* i = NULL;
	struct tile* n = NULL;
	RB_FOREACH(i, field, field){
		n = malloc(sizeof(struct tile));
		n->x = i->x; n->y = i->y;
		field_RB_INSERT(ret, n);
	}
	return ret;
}

void go_path(char* path, int* x_r, int* y_r){
	int x = 0; int y = 0; int i = 0;

	while(path[i] != '\0'){
		if(path[i] == 'n'){
			y--;
			i++;
			if(path[i] == 'w'){
				/* printf("NW "); */
				x--;
			} else if (path[i] == 'e'){
				/* printf("NE "); */
				x++;
			}
		} else if(path[i] == 's'){
			y++;
			i++;
			if(path[i] == 'w'){
				/* printf("SW "); */
				x--;
			} else if (path[i] == 'e'){
				/* printf("SE "); */
				x++;
			}
		} else if(path[i] == 'w'){
			/* printf("W "); */
			x-=2;
		} else if(path[i] == 'e'){
			/* printf("E "); */
			x+=2;
		}
		i++;
	}
	/* printf("\n"); */
	// return the calculated coordinates
	(*x_r) = x;
	(*y_r) = y;
}

struct tile* get_tile(int x, int y, struct field* field){
	struct tile s = {.x = x, .y = y};
	return field_RB_FIND(field, &s);
}

void flip_tile(int x, int y, struct field* field){
	/* printf("flipping %d|%d\n", x, y); */
	struct tile* f = get_tile(x, y, field);
	if(f == NULL){
		f = malloc(sizeof(struct tile));
		f->x = x; f->y = y;
		field_RB_INSERT(field, f);

		if(x > field->xMax) field->xMax = x;
		if(y > field->yMax) field->yMax = y;
		if(x < field->xMin) field->xMin = x;
		if(y < field->yMin) field->yMin = y;

	} else {
		field_RB_REMOVE(field, f);
	}
}

void print_field(struct field* field){
	struct tile* tile = NULL;
	for(int x=field->xMin-1; x <= field->xMax+1; x++){
		for(int y=field->yMin-1; y <= field->yMax+1; y++){
			tile = get_tile(x, y, field);
			if(x == 0 && y == 0){
				printf("%c", tile==NULL ? 'O' : 'X');
			} else {
				printf("%c", tile==NULL ? ' ' : '.');
			}
		}
		printf("|\n");
	}
}

int count(struct field* field){
	int ret = 0;
	struct tile* i = NULL;
	RB_FOREACH(i, field, field) ret++;
	return ret;
}

int part1(struct string_content* input){
	struct field* field = malloc(sizeof(struct field));
	RB_INIT(field); field->xMax = 0; field->xMin = 0; field->yMax = 0; field->yMin = 0;

	int x=0; int y=0;
	for(int i=0; i < input->length; i++){
		go_path(input->content[i], &x, &y);
		flip_tile(x, y, field);
	}
	/* print_field(field); */
	return count(field);
}

_Bool whiteRule(int x, int y, struct field* field){
	if(get_tile(x, y, field) != NULL) return 0; // no white tile

	int matches = 0;
	if(get_tile(x-2, y,   field) != NULL) matches++;
	if(get_tile(x+2, y,   field) != NULL) matches++;
	if(get_tile(x-1, y-1, field) != NULL) matches++;
	if(get_tile(x-1, y+1, field) != NULL) matches++;
	if(get_tile(x+1, y-1, field) != NULL) matches++;
	if(get_tile(x+1, y+1, field) != NULL) matches++;

	if(matches == 2) return 1;
	return 0;
}

_Bool blackRule(int x, int y, struct field* field){
	if(get_tile(x, y, field) == NULL) return 0; // no black tile

	int matches = 0;
	if(get_tile(x-2, y,   field) != NULL) matches++;
	if(get_tile(x+2, y,   field) != NULL) matches++;
	if(get_tile(x-1, y-1, field) != NULL) matches++;
	if(get_tile(x-1, y+1, field) != NULL) matches++;
	if(get_tile(x+1, y-1, field) != NULL) matches++;
	if(get_tile(x+1, y+1, field) != NULL) matches++;

	if(matches == 0 || matches > 2) return 1;
	return 0;
}

int part2(struct string_content* input){
	struct field* field1 = malloc(sizeof(struct field));
	RB_INIT(field1); field1->xMax = 0; field1->xMin = 0; field1->yMax = 0; field1->yMin = 0;

	int x=0; int y=0;
	for(int i=0; i < input->length; i++){
		go_path(input->content[i], &x, &y);
		flip_tile(x, y, field1);
	}
	/* print_field(field1); */

	for(int i=1; i <= 100; i++){
		// do one step
		struct field* field2 = RB_COPY(field1);
		for(int x=field1->xMin-1; x <= field1->xMax+1; x++){
			for(int y=field1->yMin-1; y <= field1->yMax+1; y++){
				if(whiteRule(x, y, field1) || blackRule(x, y, field1)){
					flip_tile(x, y, field2);
				}
			}
		}
		RB_DESTROY(field1);
		field1 = field2;
		/* print_field(field1); */
		/* printf("Day %d: %d\n", i, count(field1)); */
	}
	return count(field1);
}

int main() {
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./problems/day-24.dat", type);
	printf("Part1:\n%d\n", part1(input));
	free_file(input);
	
	input = read_file("./problems/day-24.dat", type);
	printf("\nPart2:\n%d\n", part2(input));
	free_file(input);
	return EXIT_SUCCESS;
}
