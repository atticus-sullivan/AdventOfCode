#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "day-20_tile.h"

/*
Advent Of Code 2020
Copyright (C) 2024  Lukas Heindl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

RB_GENERATE(image, tile, link, cmp_tile)
RB_GENERATE(neigh_set, neigh_set_ele, link, cmp_neigh)

int
cmp_tile(struct tile *a, struct tile *b)
{
	return (a->id < b->id ? -1 : a->id > b->id);
}

int
cmp_neigh(struct neigh_set_ele *a, struct neigh_set_ele *b)
{
	return (a->id < b->id ? -1 : a->id > b->id);
}


struct tile*
new_tile(char** content, int id, struct image* img)
{
	struct tile f = {.id = id};
	struct tile* ret = image_RB_FIND(img, &f);
	if(ret != NULL) return ret;

	struct neigh_set* neighT = malloc(sizeof(struct neigh_set)); RB_INIT(neighT);
	struct neigh_set* neighR = malloc(sizeof(struct neigh_set)); RB_INIT(neighR);
	struct neigh_set* neighB = malloc(sizeof(struct neigh_set)); RB_INIT(neighB);
	struct neigh_set* neighL = malloc(sizeof(struct neigh_set)); RB_INIT(neighL);

	ret = malloc(sizeof(struct tile));
	(*ret) = (struct tile){
		.id = id,
		.dir = N,
		.lock = 0, .hflip = 0, .vflip = 0,
		.neighT = neighT,
		.neighR = neighR,
		.neighB = neighB,
		.neighL = neighL,
	};
	char** content_n = malloc(10*sizeof(char*));
	for(int i=0; i < 10; i++){
		char* line = malloc(11*sizeof(char));
		content_n[i] = line;
		strncpy(line, content[i], 10);
		line[10] = '\0';
	}
	ret->content = content_n;
	image_RB_INSERT(img, ret);
	return ret;
}

void
print_neigh_set(struct neigh_set* n)
{
	struct neigh_set_ele* i = NULL;
	RB_FOREACH(i, neigh_set, n){
		printf(" {id:%d dir:%d}", i->id, i->dir);
	}
	printf("\n");
}

void
print_tile_long(struct tile* tile)
{
	printf("\n");
	printf("Tile %d:\n", tile->id);
	for(int i=0; i < 10; i++){
		printf("%s\n", tile->content[i]);
	}
	printf("\n");
}

void
print_patch(struct tile* tile, int x)
{
	for(int i=0; i < 10; i++){
		printf("\033[%dC%s\n", x*12, tile->content[i]);
	}
	printf("\033[10A");
}

void
print_tile(struct tile* tile)
{
	printf("%d: (dir:%d vflip:%d hflip:%d)\n", tile->id, tile->dir, tile->vflip, tile->hflip);
	printf(" T:");
	print_neigh_set(tile->neighT);
	printf(" R:");
	print_neigh_set(tile->neighR);
	printf(" B:");
	print_neigh_set(tile->neighB);
	printf(" L:");
	print_neigh_set(tile->neighL);
}

void
print_tile_list(struct tile_list* l)
{
	struct tile_list_ele* i = NULL;
	TAILQ_FOREACH(i, l, link){
		printf(" %d ->", i->tile->id);
	}
	printf("\n");
}

int
neigh_set_RB_SIZE(struct neigh_set* x)
{
	int ret = 0;
	struct neigh_set_ele* i = NULL;
	RB_FOREACH(i, neigh_set, x){
		ret++;
	}
	return ret;
}

// checks if top border of tile1 matches any border of tile2
// returns which border: 0:none, 1:top, 2:right, 3:bottom, 4:left
// returns which border: 0:none, -1:topR, -2:rightR, -3:bottomR, -4:leftR
int
matchesT(char** tile1, char** tile2)
{
	_Bool matchesT = 1; _Bool matchesR = 1; _Bool matchesB = 1; _Bool matchesL = 1;
	_Bool matchesTr = 1; _Bool matchesRr = 1; _Bool matchesBr = 1; _Bool matchesLr = 1;
	for(int i=0; i < 10; i++){
		if(tile1[0][i] != tile2[0][i]) matchesT = 0;
		if(tile1[0][i] != tile2[i][9]) matchesR = 0;
		if(tile1[0][i] != tile2[9][9-i]) matchesB = 0;
		if(tile1[0][i] != tile2[9-i][0]) matchesL = 0;

		if(tile1[0][i] != tile2[0][9-i]) matchesTr = 0;
		if(tile1[0][i] != tile2[9-i][9]) matchesRr = 0;
		if(tile1[0][i] != tile2[9][i]) matchesBr = 0;
		if(tile1[0][i] != tile2[i][0]) matchesLr = 0;
	}

	if(matchesT) return 1;
	if(matchesR) return 2;
	if(matchesB) return 3;
	if(matchesL) return 4;

	if(matchesTr) return -1;
	if(matchesRr) return -2;
	if(matchesBr) return -3;
	if(matchesLr) return -4;

	return 0;
}

// checks if right border of tile1 matches any border of tile2
// returns which border: -1:none, 0:top, 1:right, 2:bottom, 3:left
int
matchesB(char** tile1, char** tile2)
{
	_Bool matchesT = 1; _Bool matchesR = 1; _Bool matchesB = 1; _Bool matchesL = 1;
	_Bool matchesTr = 1; _Bool matchesRr = 1; _Bool matchesBr = 1; _Bool matchesLr = 1;
	for(int i=0; i < 10; i++){
		if(tile1[9][9-i] != tile2[0][i]) matchesT = 0;
		if(tile1[9][9-i] != tile2[i][9]) matchesR = 0;
		if(tile1[9][9-i] != tile2[9][9-i]) matchesB = 0;
		if(tile1[9][9-i] != tile2[9-i][0]) matchesL = 0;

		if(tile1[9][9-i] != tile2[0][9-i]) matchesTr = 0;
		if(tile1[9][9-i] != tile2[9-i][9]) matchesRr = 0;
		if(tile1[9][9-i] != tile2[9][i]) matchesBr = 0;
		if(tile1[9][9-i] != tile2[i][0]) matchesLr = 0;
	}

	if(matchesT) return 1;
	if(matchesR) return 2;
	if(matchesB) return 3;
	if(matchesL) return 4;

	if(matchesTr) return -1;
	if(matchesRr) return -2;
	if(matchesBr) return -3;
	if(matchesLr) return -4;

	return 0;
}

// checks if bottom border of tile1 matches any border of tile2
// returns which border: -1:none, 0:top, 1:right, 2:bottom, 3:left
int
matchesR(char** tile1, char** tile2)
{
	_Bool matchesT = 1; _Bool matchesR = 1; _Bool matchesB = 1; _Bool matchesL = 1;
	_Bool matchesTr = 1; _Bool matchesRr = 1; _Bool matchesBr = 1; _Bool matchesLr = 1;
	for(int i=0; i < 10; i++){
		if(tile1[i][9] != tile2[0][i]) matchesT = 0;
		if(tile1[i][9] != tile2[i][9]) matchesR = 0;
		if(tile1[i][9] != tile2[9][9-i]) matchesB = 0;
		if(tile1[i][9] != tile2[9-i][0]) matchesL = 0;

		if(tile1[i][9] != tile2[0][9-i]) matchesTr = 0;
		if(tile1[i][9] != tile2[9-i][9]) matchesRr = 0;
		if(tile1[i][9] != tile2[9][i]) matchesBr = 0;
		if(tile1[i][9] != tile2[i][0]) matchesLr = 0;
	}

	if(matchesT) return 1;
	if(matchesR) return 2;
	if(matchesB) return 3;
	if(matchesL) return 4;

	if(matchesTr) return -1;
	if(matchesRr) return -2;
	if(matchesBr) return -3;
	if(matchesLr) return -4;

	return 0;
}

// checks if bottom border of tile1 matches any border of tile2
// returns which border: -1:none, 0:top, 1:right, 2:bottom, 3:left
int
matchesL(char** tile1, char** tile2)
{
	_Bool matchesT = 1; _Bool matchesR = 1; _Bool matchesB = 1; _Bool matchesL = 1;
	_Bool matchesTr = 1; _Bool matchesRr = 1; _Bool matchesBr = 1; _Bool matchesLr = 1;
	for(int i=0; i < 10; i++){
		if(tile1[9-i][0] != tile2[0][i]) matchesT = 0;
		if(tile1[9-i][0] != tile2[i][9]) matchesR = 0;
		if(tile1[9-i][0] != tile2[9][9-i]) matchesB = 0;
		if(tile1[9-i][0] != tile2[9-i][0]) matchesL = 0;

		if(tile1[9-i][0] != tile2[0][9-i]) matchesTr = 0;
		if(tile1[9-i][0] != tile2[9-i][9]) matchesRr = 0;
		if(tile1[9-i][0] != tile2[9][i]) matchesBr = 0;
		if(tile1[9-i][0] != tile2[i][0]) matchesLr = 0;
	}

	if(matchesT) return 1;
	if(matchesR) return 2;
	if(matchesB) return 3;
	if(matchesL) return 4;

	if(matchesTr) return -1;
	if(matchesRr) return -2;
	if(matchesBr) return -3;
	if(matchesLr) return -4;

	return 0;
}

// helper functions for the neigh_set_ele->dir field transformation
int rol(int dir){return dir;}
int ror(int dir){return dir;}
int hflip(int dir){return -dir;}
int vflip(int dir){return -dir;}

// do transformations on the neighbour sets only
void
rotateCounterClock_neigh(struct neigh_set_ele* neigh)
{
	/* printf("Rotate %d counter clockwise\n", neigh->id); */
	struct neigh_set_ele* i = NULL;
	RB_FOREACH(i, neigh_set, neigh->tile->neighT){ i->dir = rol(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighR){ i->dir = rol(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighB){ i->dir = rol(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighL){ i->dir = rol(i->dir); }

	struct neigh_set* tmp = neigh->tile->neighT;
	neigh->tile->neighT = neigh->tile->neighR;
	neigh->tile->neighR = neigh->tile->neighB;
	neigh->tile->neighB = neigh->tile->neighL;
	neigh->tile->neighL = tmp;

	neigh->dir = rol(neigh->dir);

	switch(neigh->tile->dir){
		case N: neigh->tile->dir = W; break;
		case E: neigh->tile->dir = N; break;
		case S: neigh->tile->dir = E; break;
		case W: neigh->tile->dir = S; break;
	}
	_Bool tmp_flip = neigh->tile->vflip;
	neigh->tile->vflip = neigh->tile->hflip;
	neigh->tile->hflip = tmp_flip;
}

void
rotateClock_neigh(struct neigh_set_ele* neigh)
{
	/* printf("Rotate %d clockwise\n", neigh->id); */
	struct neigh_set_ele* i = NULL;
	RB_FOREACH(i, neigh_set, neigh->tile->neighT){ i->dir = ror(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighR){ i->dir = ror(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighB){ i->dir = ror(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighL){ i->dir = ror(i->dir); }

	struct neigh_set* tmp = neigh->tile->neighT;
	neigh->tile->neighT = neigh->tile->neighL;
	neigh->tile->neighL = neigh->tile->neighB;
	neigh->tile->neighB = neigh->tile->neighR;
	neigh->tile->neighR = tmp;

	neigh->dir = ror(neigh->dir);

	switch(neigh->tile->dir){
		case N: neigh->tile->dir = E; break;
		case E: neigh->tile->dir = S; break;
		case S: neigh->tile->dir = W; break;
		case W: neigh->tile->dir = N; break;
	}
	_Bool tmp_flip = neigh->tile->vflip;
	neigh->tile->vflip = neigh->tile->hflip;
	neigh->tile->hflip = tmp_flip;
}

void
vflip_neigh(struct neigh_set_ele* neigh)
{
	/* printf("Flip %d vertically\n", neigh->id); */
	struct neigh_set_ele* i = NULL;
	RB_FOREACH(i, neigh_set, neigh->tile->neighT){ i->dir = vflip(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighB){ i->dir = vflip(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighL){ i->dir = vflip(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighR){ i->dir = vflip(i->dir); }

	struct neigh_set* tmp = neigh->tile->neighR;
	neigh->tile->neighR = neigh->tile->neighL;
	neigh->tile->neighL = tmp;

	neigh->dir = vflip(neigh->dir);

	neigh->tile->vflip = neigh->tile->vflip ^ 1;
}

void
hflip_neigh(struct neigh_set_ele* neigh)
{
	/* printf("Flip %d horizontally\n", neigh->id); */
	struct neigh_set_ele* i = NULL;
	RB_FOREACH(i, neigh_set, neigh->tile->neighR){ i->dir = hflip(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighL){ i->dir = hflip(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighB){ i->dir = hflip(i->dir); }
	RB_FOREACH(i, neigh_set, neigh->tile->neighT){ i->dir = hflip(i->dir); }

	struct neigh_set* tmp = neigh->tile->neighT;
	neigh->tile->neighT = neigh->tile->neighB;
	neigh->tile->neighB = tmp;

	neigh->dir = hflip(neigh->dir);

	neigh->tile->hflip = neigh->tile->hflip ^ 1;
}

void
undo_trans(struct neigh_set_ele* neigh)
{
	/* printf("undo\n"); */
	if(neigh->tile->hflip) hflip_neigh(neigh);
	if(neigh->tile->vflip) vflip_neigh(neigh);

	enum orientation dir = neigh->tile->dir;
	switch(dir){
		case S:
			rotateClock_neigh(neigh);
			// fallthrough
		case W:
			rotateClock_neigh(neigh);
			break;
		case E:
			rotateCounterClock_neigh(neigh);
			break;
		case N: break;
	}

	assert(neigh->tile->hflip == 0 && "undo trans failed");
	assert(neigh->tile->vflip == 0 && "undo trans failed");
	assert(neigh->tile->dir == N && "undo trans failed");
}

void
fit_bottom(struct neigh_set_ele* neigh)
{
	switch(neigh->dir){
		case -4: // left
			rotateCounterClock_neigh(neigh);
			break;
		case -3: // bottom
			hflip_neigh(neigh);
			vflip_neigh(neigh);
			break;
		case -2: // right reverse
			rotateCounterClock_neigh(neigh);
			break;
		case -1: // top reverse
			break;
		case 1: // top
			vflip_neigh(neigh);
			break;
		case 2: // right
			rotateCounterClock_neigh(neigh);
			vflip_neigh(neigh);
			break;
		case 3: // bottom reverse
			hflip_neigh(neigh);
			break;
		case 4: // left reverse
			rotateClock_neigh(neigh);
			vflip_neigh(neigh);
			break;
	}
}

void
fit_right(struct neigh_set_ele* neigh)
{
	switch(neigh->dir){
		case -4: // left
			break;
		case -3: // bottom
			rotateClock_neigh(neigh);
			break;
		case -2: // right reverse
			vflip_neigh(neigh);
			hflip_neigh(neigh);
			break;
		case -1: // top reverse
			rotateCounterClock_neigh(neigh);
			break;
		case 1: // top
			rotateCounterClock_neigh(neigh);
			hflip_neigh(neigh);
			break;
		case 2: // right
			vflip_neigh(neigh);
			break;
		case 3: // bottom reverse
			rotateClock_neigh(neigh);
			hflip_neigh(neigh);
			break;
		case 4: // left reverse
			hflip_neigh(neigh);
			break;
	}
}

_Bool
check_top_neigh(struct neigh_set* neigh, struct tile* tile)
{
	struct neigh_set_ele s = {.id = tile->id};
	struct neigh_set_ele* f = neigh_set_RB_FIND(neigh, &s);
	if(f == NULL) return 0;
	/* print_tile(f->tile); */

	switch(f->dir){
		case -4:
			return
				(tile->dir == W && !tile->vflip && !tile->hflip) ||
				(tile->dir == E && tile->vflip && tile->hflip)
				;
		case -3:
			return
				(tile->dir == N && !tile->vflip && !tile->hflip)
				;
		case -2:
			return
				(tile->dir == W && tile->vflip && tile->hflip) ||
				(tile->dir == E && !tile->vflip && !tile->hflip)
				;
		case -1:
			return
				(tile->dir == S && !tile->vflip && !tile->hflip) ||
				(tile->dir == N && tile->vflip && tile->hflip)
				;
		case  1:
			return
				(tile->dir == N && !tile->vflip && tile->hflip) ||
				(tile->dir == S && tile->vflip && !tile->hflip)
				;
		case  2:
			return
				(tile->dir == E && tile->vflip && !tile->hflip) ||
				(tile->dir == W && !tile->vflip && tile->hflip)
				;
		case  3:
			return
				(tile->dir == N && tile->vflip && !tile->hflip)
				;
		case  4:
			return
				(tile->dir == W && tile->vflip && !tile->hflip) ||
				(tile->dir == E && !tile->vflip && tile->hflip)
				;
	}
	return 0;
}

// do transformations on the content string array
void
hflip_content(struct tile* tile)
{
	char** content_n = malloc(10*sizeof(char*));
	for(int i=0; i < 10; i++){
		char* line = malloc(11*sizeof(char));
		content_n[i] = line;
		for(int j=0; j < 10; j++){
			line[j] = tile->content[9-i][j];
		}
		line[10] = '\0';
	}
	
	for(int i=0; i < 10; i++){
		free(tile->content[i]);
	}
	tile->content = content_n;

	tile->hflip = tile->hflip ^ 1;
}

void
vflip_content(struct tile* tile)
{
	char** content_n = malloc(10*sizeof(char*));
	for(int i=0; i < 10; i++){
		char* line = malloc(11*sizeof(char));
		content_n[i] = line;
		for(int j=0; j < 10; j++){
			line[j] = tile->content[i][9-j];
		}
		line[10] = '\0';
	}
	
	for(int i=0; i < 10; i++){
		free(tile->content[i]);
	}
	tile->content = content_n;

	tile->vflip = tile->vflip ^ 1;
}

void
rotateClock_content(struct tile* tile)
{
	char** content_n = malloc(10*sizeof(char*));
	for(int i=0; i < 10; i++){
		char* line = malloc(11*sizeof(char));
		content_n[i] = line;
		for(int j=0; j < 10; j++){
			line[j] = tile->content[9-j][i];
		}
		line[10] = '\0';
	}
	
	for(int i=0; i < 10; i++){
		free(tile->content[i]);
	}
	tile->content = content_n;

	switch(tile->dir){
		case N: tile->dir = E; break;
		case E: tile->dir = S; break;
		case S: tile->dir = W; break;
		case W: tile->dir = N; break;
	}
	/* _Bool tmp_flip = tile->vflip; */
	/* tile->vflip = tile->hflip; */
	/* tile->hflip = tmp_flip; */
}

void
rotateCounterClock_content(struct tile* tile)
{
	char** content_n = malloc(10*sizeof(char*));
	for(int i=0; i < 10; i++){
		char* line = malloc(11*sizeof(char));
		content_n[i] = line;
		for(int j=0; j < 10; j++){
			line[j] = tile->content[j][9-i];
		}
		line[10] = '\0';
	}
	
	for(int i=0; i < 10; i++){
		free(tile->content[i]);
	}
	tile->content = content_n;

	switch(tile->dir){
		case N: tile->dir = W; break;
		case E: tile->dir = N; break;
		case S: tile->dir = E; break;
		case W: tile->dir = S; break;
	}
	/* _Bool tmp_flip = tile->vflip; */
	/* tile->vflip = tile->hflip; */
	/* tile->hflip = tmp_flip; */
}

void
follow_trans_log(struct tile* tile)
{
	enum orientation dir = tile->dir;
	switch(dir){
		case S:
			/* printf("follow %d: clock\n", tile->id); */
			rotateCounterClock_content(tile);
			// fallthrough
		case W:
			/* printf("follow %d: clock\n", tile->id); */
			rotateCounterClock_content(tile);
			break;
		case E:
			/* printf("follow %d: counter\n", tile->id); */
			rotateClock_content(tile);
			break;
		case N: break;
	}

	if(tile->hflip){
		/* printf("follow %d: hflip\n", tile->id); */
		hflip_content(tile);
	}
	if(tile->vflip){
		/* printf("follow %d: vflip\n", tile->id); */
		vflip_content(tile);
	}

	/* assert(tile->hflip == 0 && "follow log failed"); */
	/* assert(tile->vflip == 0 && "follow log failed"); */
	/* assert(tile->dir == N && "follow log failed"); */
}

struct tile_list*
arrange_tile_go_down(
		struct tile* active,
		struct tile_list* this_row
		)
{
	struct tile_list* empty_row = malloc(sizeof(struct tile_list));
	TAILQ_INIT(empty_row);

	if(neigh_set_RB_SIZE(active->neighB) == 0){
		/* printf("%d: hit bottom edge\n", active->id); */
		// bottom edge was met -> finished
		struct tile_list* r = malloc(sizeof(struct tile_list));
		TAILQ_INIT(r);
		struct tile_list_ele* active_r = malloc(sizeof(struct tile_list_ele));
		active_r->tile = active;
		TAILQ_INSERT_HEAD(r, active_r, link);
		return r;
	}

	struct neigh_set_ele* i = NULL;
	RB_FOREACH(i, neigh_set, active->neighB){
		if(i->tile->lock) continue;
		/* printf("%d: check %d\n", active->id, i->id); */
		// rotate/flip i to fit bottom matching
		fit_bottom(i);
		assert(TAILQ_EMPTY(empty_row) && "empty row wasn't empty");
		struct tile_list* r = arrange_tile(i->tile, this_row, empty_row, i->tile);
		if(r != NULL){
			struct tile_list_ele* active_r = malloc(sizeof(struct tile_list_ele));
			active_r->tile = active;
			TAILQ_INSERT_HEAD(r, active_r, link);
			return r;
		}
		// undo transformation
		undo_trans(i);
	}
	return NULL;
}

struct tile_list*
arrange_tile_go_right(
		struct tile* active,
		struct tile_list* last_row,
		struct tile_list* this_row,
		struct tile* anchor
		)
{
	active->lock = 1;
	struct tile_list_ele* tr = malloc(sizeof(struct tile_list_ele));
	tr->tile = active;
	TAILQ_INSERT_TAIL(this_row, tr, link);

	if(last_row != NULL){
		tr = TAILQ_FIRST(last_row);
		TAILQ_REMOVE(last_row, tr, link);
		/* printf("%d last_row:", active->id); print_tile_list(last_row); */
	}

	if(neigh_set_RB_SIZE(active->neighR) == 0 && (last_row == NULL || TAILQ_EMPTY(last_row))){
		/* printf("%d: hit right edge\n", active->id); */
		// right edge was met -> go to the next row
		struct tile_list* r = arrange_tile(anchor, last_row, this_row, anchor);
		if(r != NULL){
			struct tile_list_ele* active_r = malloc(sizeof(struct tile_list_ele));
			active_r->tile = active;
			TAILQ_INSERT_HEAD(r, active_r, link);
			return r;
		}
	} else {
		if(last_row != NULL && TAILQ_EMPTY(last_row)) goto right_cleanup; // abort if shape does not fit

		struct neigh_set_ele* i = NULL;
		RB_FOREACH(i, neigh_set, active->neighR){
			if(i->tile->lock) continue;
			/* printf("%d: check %d\n", active->id, i->id); */
			/* print_tile(i->tile); */
			// rotate/flip i to fit right matching
			fit_right(i);
			// check if i->tile fits to head of last_row
			/* print_tile(i->tile); */
			if(last_row == NULL || check_top_neigh(i->tile->neighT, TAILQ_FIRST(last_row)->tile)){
				struct tile_list* r = arrange_tile(i->tile, last_row, this_row, anchor);
				if(r != NULL){
					struct tile_list_ele* active_r = malloc(sizeof(struct tile_list_ele));
					active_r->tile = active;
					TAILQ_INSERT_HEAD(r, active_r, link);
					return r;
				}
			}
			// undo transformation
			undo_trans(i);
		}
	}
right_cleanup:
	if(last_row != NULL)
		TAILQ_INSERT_HEAD(last_row, tr, link);

	TAILQ_REMOVE(this_row, TAILQ_LAST(this_row, tile_list), link);
	active->lock = 0;
	return NULL;
}

// 0:none, 1:top, 2:right, 3:bottom, 4:left // negative means reverse
struct tile_list*
arrange_tile(
		struct tile* active,
		struct tile_list* last_row,
		struct tile_list* this_row,
		struct tile* anchor /*tile at the beginning of the row*/)
{
	if(active->lock){
		/* printf("%d: go down\n", active->id); */
		/* print_tile(active); */
		// new row -> go down
		return arrange_tile_go_down(active, this_row);
	} else {
		/* printf("%d: go right\n", active->id); */
		/* print_tile(active); */
		// go right
		return arrange_tile_go_right(active, last_row, this_row, anchor);
	}
	/* printf("%d: Go back\n", active->id); */
	return NULL;
}

struct tile_list*
arrange_tile_corner(struct neigh_set_ele* cor)
{
	while (neigh_set_RB_SIZE(cor->tile->neighB) == 0 || neigh_set_RB_SIZE(cor->tile->neighR) == 0)
		rotateCounterClock_neigh(cor);

	struct tile_list* this_row = malloc(sizeof(struct tile_list));
	TAILQ_INIT(this_row);

	struct tile_list* r = arrange_tile(cor->tile, NULL, this_row, cor->tile);
	if(r != NULL){
		return r;
	}
	// nothing matched
	return NULL;
}

struct tile_list*
arrange_tiles(struct tile* corner)
{
	struct neigh_set_ele cor_tmp = {.tile = corner, .dir = 1, .id = corner->id};
	struct neigh_set_ele* cor = &cor_tmp;
	struct tile_list* ret = NULL;

	ret = arrange_tile_corner(cor); // nothing flipped
	if(ret != NULL)
		return ret;

	vflip_neigh(cor);
	ret = arrange_tile_corner(cor); // vflip
	if(ret != NULL)
		return ret;

	hflip_neigh(cor);
	ret = arrange_tile_corner(cor); // vflip + hflip
	if(ret != NULL)
		return ret;

	vflip_neigh(cor);
	ret = arrange_tile_corner(cor); // hflip
	if(ret != NULL)
		return ret;
	return NULL;
}

/* struct image* */
void
gen_tiles(struct string_content* input,
		struct tile** corner0, struct tile** corner1, struct tile** corner2, struct tile** corner3)
{
	struct image* img = malloc(sizeof(struct image)); RB_INIT(img);
	int corners_ind = 0;
	struct tile* corners[4]; // TL, TR, BR, BL
	struct neigh_set_ele* neighJ = NULL;

	for(int i=0; i < input->length; i++){
		if(!strncmp(input->content[i], "Tile ", 5)){
			struct tile* tileI = new_tile(input->content+i+1, atoi(input->content[i]+5), img);
			for(int j=0; j < input->length; j++){
				if(i!=j && !strncmp(input->content[j], "Tile ", 5)){
					struct tile* tileJ = new_tile(input->content+j+1, atoi(input->content[j]+5), img);

					int r = matchesT(input->content+i+1, input->content+j+1);
					if(r != 0){
						/* printf("%dT -> %d %d\n", tileI->id, tileJ->id, r); */
						neighJ = malloc(sizeof(struct neigh_set_ele));
						(*neighJ) = (struct neigh_set_ele) {.id = tileJ->id, .tile = tileJ, .dir = r};
						neigh_set_RB_INSERT(tileI->neighT, neighJ);
					}

					r = matchesR(input->content+i+1, input->content+j+1);
					if(r != 0){
						/* printf("%dR -> %d %d\n", tileI->id, tileJ->id, r); */
						neighJ = malloc(sizeof(struct neigh_set_ele));
						(*neighJ) = (struct neigh_set_ele) {.id = tileJ->id, .tile = tileJ, .dir = r};
						neigh_set_RB_INSERT(tileI->neighR, neighJ);
					}

					r = matchesB(input->content+i+1, input->content+j+1);
					if(r != 0){
						/* printf("%dB -> %d %d\n", tileI->id, tileJ->id, r); */
						neighJ = malloc(sizeof(struct neigh_set_ele));
						(*neighJ) = (struct neigh_set_ele) {.id = tileJ->id, .tile = tileJ, .dir = r};
						neigh_set_RB_INSERT(tileI->neighB, neighJ);
					}

					r = matchesL(input->content+i+1, input->content+j+1);
					if(r != 0){
						/* printf("%dL -> %d %d\n", tileI->id, tileJ->id, r); */
						neighJ = malloc(sizeof(struct neigh_set_ele));
						(*neighJ) = (struct neigh_set_ele) {.id = tileJ->id, .tile = tileJ, .dir = r};
						neigh_set_RB_INSERT(tileI->neighL, neighJ);
					}
				}
			}
			int nT = neigh_set_RB_SIZE(tileI->neighT);
			int nR = neigh_set_RB_SIZE(tileI->neighR);
			int nB = neigh_set_RB_SIZE(tileI->neighB);
			int nL = neigh_set_RB_SIZE(tileI->neighL);

			if(nT == 0 && nL == 0) corners[corners_ind++] = tileI;
			if(nT == 0 && nR == 0) corners[corners_ind++] = tileI;
			if(nB == 0 && nL == 0) corners[corners_ind++] = tileI;
			if(nB == 0 && nR == 0) corners[corners_ind++] = tileI;
		}
	}

	(*corner0) = corners[0];
	(*corner1) = corners[1];
	(*corner2) = corners[2];
	(*corner3) = corners[3];
	/* printf("Corners: %d, %d, %d, %d\n", corners[0]->id, corners[1]->id, corners[2]->id, corners[3]->id); */
	/* return img; */
}
