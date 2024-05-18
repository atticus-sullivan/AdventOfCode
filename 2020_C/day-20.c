#include <stdio.h>
#include <stdlib.h>

#include "read_file.h"

#include "day-20_img.h"
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

int
part2(struct string_content* input)
{
	struct tile* corner0 = NULL; // out parameter
	struct tile* corner1 = NULL; // out parameter
	struct tile* corner2 = NULL; // out parameter
	struct tile* corner3 = NULL; // out parameter
	/* struct image* img = gen_tiles(input, &corner0, &corner1, &corner2, &corner3); */
	gen_tiles(input, &corner0, &corner1, &corner2, &corner3);

	printf("Part1:\nCorners %ld", (long)corner0->id * corner1->id * corner2->id * corner3->id);

	struct tile_list* ret = arrange_tiles(corner0);
	/* print_tile_list(ret); */

	/* printf("\n"); */

	struct img* i = build_image(ret);
#ifdef DEBUG
	print_image_ids(i);
	print_image_str(i);

	printf("\n\n");
#endif

	struct img_str* r = find_monster(i->str);

#ifdef DEBUG
	img_str_PRINT(r);
#endif

	return count(r);
}

int
main()
{
	char* type;
	(void)type;

	struct string_content *input;
	input = read_file("./problems/day-20.dat", type);
	printf("\n\nPart2:\nWater roughness %d\n", part2(input));
	free_file(input);

	return EXIT_SUCCESS;
}

// TODO cleanup memory
