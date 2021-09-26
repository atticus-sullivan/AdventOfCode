#ifndef _DAY_20_tile_H_
#define _DAY_20_tile_H_
#include <bsd/sys/tree.h>
#include <bsd/sys/queue.h>
#include <stdbool.h>
#include <stdlib.h>

#include "read_file.h"

enum orientation {
	N, E, S, W
};

struct neigh_set_ele {
	int id; // key
	struct tile* tile;
	int dir; // which border of this->tile is matching: 0:none, 1:top, 2:right, 3:bottom, 4:left // negative means reverse
	RB_ENTRY(neigh_set_ele) link;
};
int cmp_neigh(struct neigh_set_ele *a, struct neigh_set_ele *b);

RB_HEAD(neigh_set, neigh_set_ele);
RB_PROTOTYPE(neigh_set, neigh_set_ele, link, cmp_neigh)

struct tile {
	int id; // key
	char** content; // array of dimension 10x10
	enum orientation dir;
	_Bool lock; _Bool hflip; _Bool vflip;
	struct neigh_set* neighT;
	struct neigh_set* neighR;
	struct neigh_set* neighB;
	struct neigh_set* neighL;
	RB_ENTRY(tile) link;
};
int cmp_tile(struct tile *a, struct tile *b);

RB_HEAD(image, tile);
RB_PROTOTYPE(image, tile, link, cmp_tile)

struct tile_list_ele {
	struct tile* tile;
	TAILQ_ENTRY(tile_list_ele) link;
};
TAILQ_HEAD(tile_list, tile_list_ele);

/* struct image* */
void
gen_tiles(struct string_content* input,
		struct tile** corner0, struct tile** corner1, struct tile** corner2, struct tile** corner3);


struct tile_list*
arrange_tiles(struct tile* corner);

struct tile_list*
arrange_tile(
		struct tile* active,
		struct tile_list* last_row,
		struct tile_list* this_row,
		struct tile* anchor /*tile at the beginning of the row*/);

void
print_tile_list(struct tile_list* l);

void
print_patch(struct tile* tile, int x);

void
follow_trans_log(struct tile* tile);

#endif
