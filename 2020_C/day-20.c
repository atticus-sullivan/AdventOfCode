#include <stdio.h>
#include <stdlib.h>

#include "read_file.h"

#include "day-20_img.h"
#include "day-20_tile.h"

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
