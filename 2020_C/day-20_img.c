#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "./day-20_img.h"

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

RB_GENERATE(img_str, img_str_point, link, cmp_point);

int
cmp_point(struct img_str_point *a, struct img_str_point *b)
{
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

void
print_image_ids(struct img* img)
{
	struct img_line* i = NULL;
	struct img_line_ele* j = NULL;
	TAILQ_FOREACH(i, img, link){
		TAILQ_FOREACH(j, i, link){
			printf("|%d|", j->tile->id);
		}
		printf("\n");
	}
}

void
print_image_str(struct img* img)
{
	struct img_line* i = NULL;
	struct img_line_ele* j = NULL;
	int x = 0;
	TAILQ_FOREACH(i, img, link){
		TAILQ_FOREACH(j, i, link){
			print_patch(j->tile, x++);
		}
		printf("\033[10B\n");
		x = 0;
	}
}

struct img_str_point*
img_str_GET_AT(struct img_str* head, int x, int y)
{
	struct img_str_point s = {.x = x, .y = y};
	struct img_str_point* f = img_str_RB_FIND(head, &s);
	if(f == NULL)
		return NULL;
	else
		return f;
}

_Bool
find_monser_at(struct img_str* head, int x, int y)
{
	struct img_str_point* p1  = img_str_GET_AT(head, x,    y+1);
	struct img_str_point* p2  = img_str_GET_AT(head, x+1,  y+2);
	struct img_str_point* p3  = img_str_GET_AT(head, x+4,  y+2);
	struct img_str_point* p4  = img_str_GET_AT(head, x+5,  y+1);
	struct img_str_point* p5  = img_str_GET_AT(head, x+6,  y+1);
	struct img_str_point* p6  = img_str_GET_AT(head, x+7,  y+2);
	struct img_str_point* p7  = img_str_GET_AT(head, x+10, y+2);
	struct img_str_point* p8  = img_str_GET_AT(head, x+11, y+1);
	struct img_str_point* p9  = img_str_GET_AT(head, x+12, y+1);
	struct img_str_point* p10 = img_str_GET_AT(head, x+13, y+2);
	struct img_str_point* p11 = img_str_GET_AT(head, x+16, y+2);
	struct img_str_point* p12 = img_str_GET_AT(head, x+17, y+1);
	struct img_str_point* p13 = img_str_GET_AT(head, x+18, y  );
	struct img_str_point* p14 = img_str_GET_AT(head, x+18, y+1);
	struct img_str_point* p15 = img_str_GET_AT(head, x+19, y+1);

	if(
			p1  != NULL &&  p1->c == '#' &&
			p2  != NULL &&  p2->c == '#' &&
			p3  != NULL &&  p3->c == '#' &&
			p4  != NULL &&  p4->c == '#' &&
			p5  != NULL &&  p5->c == '#' &&
			p6  != NULL &&  p6->c == '#' &&
			p7  != NULL &&  p7->c == '#' &&
			p8  != NULL &&  p8->c == '#' &&
			p9  != NULL &&  p9->c == '#' &&
			p10 != NULL && p10->c == '#' &&
			p11 != NULL && p11->c == '#' &&
			p12 != NULL && p12->c == '#' &&
			p13 != NULL && p13->c == '#' &&
			p14 != NULL && p14->c == '#' &&
			p15 != NULL && p15->c == '#'
	  ){
		p1->c = 'O';
		p2->c = 'O';
		p3->c = 'O';
		p4->c = 'O';
		p5->c = 'O';
		p6->c = 'O';
		p7->c = 'O';
		p8->c = 'O';
		p9->c = 'O';
		p10->c = 'O';
		p11->c = 'O';
		p12->c = 'O';
		p13->c = 'O';
		p14->c = 'O';
		p15->c = 'O';
		return 1;
	}
	return 0;
}

int
count(struct img_str* head)
{
	int ret = 0;
	struct img_str_point* i = NULL;
	RB_FOREACH(i, img_str, head){
		if(i->c == '#')
			ret++;
	}
	return ret;
}

_Bool
find_monster_dir(struct img_str* head)
{
	_Bool found = 0;
	for(int y=head->yMin; y <= head->yMax; y++){
		for(int x=head->xMin; x <= head->xMax; x++){
			if(find_monser_at(head, x, y))
				found = 1;
		}
	}
	return found;
}

void
img_str_PRINT_AT(struct img_str* head, int x, int y)
{
	struct img_str_point s = {.x = x, .y = y};
	struct img_str_point* f = img_str_RB_FIND(head, &s);
	if(f == NULL)
		printf(" ");
	else
		printf("%c", f->c);
}

void
img_str_PRINT(struct img_str* head)
{
	for(int y=head->yMin; y <= head->yMax; y++){
		for(int x=head->xMin; x <= head->xMax; x++){
			img_str_PRINT_AT(head, x, y);
		}
		printf("\n");
		
	}
}

void
img_str_INSERT_AT(struct img_str* head, int x, int y, char c)
{
	struct img_str_point* ins = malloc(sizeof(struct img_str_point));
	ins->x = x; ins->y = y; ins->c = c;

	if(x > head->xMax) head->xMax = x;
	if(x < head->xMin) head->xMin = x;

	if(y > head->yMax) head->yMax = y;
	if(y < head->yMin) head->yMin = y;

	struct img_str_point* r = img_str_RB_INSERT(head, ins);
	assert(r == NULL && "Overwriting pixel in image");
}

void
img_str_INSERT(struct img_str* head, int x, int y, struct tile* tile)
{
	for(int i=1; i < 9; i++){
		for(int j=1; j < 9; j++){
			img_str_INSERT_AT(head, x*8+j, y*8+i, tile->content[i][j]);
		}
	}
}

struct img*
build_image(struct tile_list* in)
{
	struct img* ret = malloc(sizeof(struct img));
	ret->str = malloc(sizeof(struct img_str));
	ret->str->xMax = 0; ret->str->xMin = 0;
	ret->str->yMax = 0; ret->str->yMin = 0;
	RB_INIT(ret->str);
	TAILQ_INIT(ret);

	struct img_line* line = malloc(sizeof(struct img_line));
	TAILQ_INIT(line);
	TAILQ_INSERT_TAIL(ret, line, link);

	struct tile_list_ele* i = NULL;
	int x = 0; int y = 0;
	TAILQ_FOREACH(i, in, link){
		follow_trans_log(i->tile);
		if(i->tile->lock){
			struct img_line_ele* ele = malloc(sizeof(struct img_line_ele));
			ele->tile = i->tile;
			TAILQ_INSERT_TAIL(line, ele, link);
			i->tile->lock = 0;
			img_str_INSERT(ret->str, x, y, i->tile);
			x = x+1;
		} else {
			line = malloc(sizeof(struct img_line));
			TAILQ_INIT(line);
			TAILQ_INSERT_TAIL(ret, line, link);
			y = y+1; x = 0;
		}
	}
	return ret;
}

struct img_str*
img_str_COPY(struct img_str* head)
{
	struct img_str* ret = malloc(sizeof(struct img_str));
	RB_INIT(ret);
	ret->xMax = head->xMax; ret->xMin = head->xMin;
	ret->yMax = head->yMax; ret->yMin = head->yMin;

	struct img_str_point* i = NULL;
	RB_FOREACH(i, img_str, head){
		img_str_INSERT_AT(ret, i->x, i->y, i->c);
	}
	return ret;
}

struct img_str*
img_str_counter(struct img_str* head)
{
	struct img_str* ret = malloc(sizeof(struct img_str));
	ret->xMin = 0; ret->xMax = 0;
	ret->yMin = 0; ret->yMax = 0;
	RB_INIT(ret);

	int min = head->xMin < head->yMin ? head->xMin : head->yMin;
	int max = head->xMax > head->yMax ? head->xMax : head->yMax;

	for(int y=min; y <= max; y++){
		for(int x=min; x <= max; x++){
			struct img_str_point* r = img_str_GET_AT(head, x, y);
			if(r == NULL) continue;
			img_str_INSERT_AT(ret, y, max-x, r->c);
		}
	}

	return ret;
}

struct img_str*
img_str_clock(struct img_str* head)
{
	struct img_str* ret = malloc(sizeof(struct img_str));
	ret->xMin = 0; ret->xMax = 0;
	ret->yMin = 0; ret->yMax = 0;
	RB_INIT(ret);

	int min = head->xMin < head->yMin ? head->xMin : head->yMin;
	int max = head->xMax > head->yMax ? head->xMax : head->yMax;

	for(int y=min; y <= max; y++){
		for(int x=min; x <= max; x++){
			struct img_str_point* r = img_str_GET_AT(head, x, y);
			if(r == NULL) continue;
			img_str_INSERT_AT(ret, max-y, x, r->c);
		}
	}

	return ret;
}

struct img_str*
img_str_vflip(struct img_str* head)
{
	struct img_str* ret = malloc(sizeof(struct img_str));
	ret->xMin = 0; ret->xMax = 0;
	ret->yMin = 0; ret->yMax = 0;
	RB_INIT(ret);

	int min = head->xMin < head->yMin ? head->xMin : head->yMin;
	int max = head->xMax > head->yMax ? head->xMax : head->yMax;

	for(int y=min; y <= max; y++){
		for(int x=min; x <= max; x++){
			struct img_str_point* r = img_str_GET_AT(head, x, y);
			if(r == NULL) continue;
			img_str_INSERT_AT(ret, max-x, y, r->c);
		}
	}

	return ret;
}

struct img_str*
img_str_hflip(struct img_str* head)
{
	struct img_str* ret = malloc(sizeof(struct img_str));
	ret->xMin = 0; ret->xMax = 0;
	ret->yMin = 0; ret->yMax = 0;
	RB_INIT(ret);

	int min = head->xMin < head->yMin ? head->xMin : head->yMin;
	int max = head->xMax > head->yMax ? head->xMax : head->yMax;

	for(int y=min; y <= max; y++){
		for(int x=min; x <= max; x++){
			struct img_str_point* r = img_str_GET_AT(head, x, y);
			if(r == NULL) continue;
			img_str_INSERT_AT(ret, x, max-y, r->c);
		}
	}

	return ret;
}

struct img_str*
find_monster(struct img_str* head)
{
	struct img_str* work = img_str_COPY(head);
	if(find_monster_dir(work)) // NORHT
		return work;
	work = img_str_clock(work);
	if(find_monster_dir(work)) // EAST
		return work;
	work = img_str_clock(work); // SOUTH
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // WEST
	if(find_monster_dir(work))
		return work;
	/* return work; */ // TODO

	work = img_str_vflip(head); // NORTH VFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // EAST VFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // SOUTH VFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // WEST VFLIP
	if(find_monster_dir(work))
		return work;

	work = img_str_hflip(head); // NORTH HFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // EAST HFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // SOUTH HFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // WEST HFLIP
	if(find_monster_dir(work))
		return work;

	work = img_str_hflip(head);
	work = img_str_vflip(work); // NORTH HFLIP VFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // EAST HFLIP VFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // SOUTH HFLIP VFLIP
	if(find_monster_dir(work))
		return work;
	work = img_str_clock(work); // WEST HFLIP VFLIP
	if(find_monster_dir(work))
		return work;

	return NULL;
}
