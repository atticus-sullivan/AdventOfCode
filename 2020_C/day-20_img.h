#ifndef _DAY_20_img_H_
#define _DAY_20_img_H_
#include <bsd/sys/tree.h>

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

struct img_str_point {
	int x; int y; // keys
	char c;
	RB_ENTRY(img_str_point) link;
};
int cmp_point(struct img_str_point *a, struct img_str_point *b);

struct img_str {
	struct img_str_point *rbh_root;
	int xMin; int xMax;
	int yMin; int yMax;
};
RB_PROTOTYPE(img_str, img_str_point, link, cmp_point);

struct img_line_ele {
	struct tile* tile;
	TAILQ_ENTRY(img_line_ele) link;
};
struct img_line {
	struct img_line_ele *tqh_first;
	struct img_line_ele **tqh_last;
	TRACEBUF
	TAILQ_ENTRY(img_line) link;
};
struct img {
	struct img_str* str;
	struct img_line *tqh_first;
	struct img_line **tqh_last;
	TRACEBUF
};

struct img*
build_image(struct tile_list* in);

void
print_image_ids(struct img* img);

void
print_image_str(struct img* img);

struct img_str*
find_monster(struct img_str* head);

int
count(struct img_str* head);

void
img_str_PRINT(struct img_str* head);

#endif
