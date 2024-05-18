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

#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "aocutils.h"

struct map_t
{
	std::set<std::pair<int,int>> m;
	int maxX, minX;
	int maxY, minY;
};

auto parse(std::ifstream& ifs)
{
	map_t ret{};
	while(!ifs.eof())
	{
		int x_old{}, y_old{};
		ifs >> x_old;
		if(ifs.eof()) break;
		ifs.ignore(1); // ,
		ifs >> y_old;
		ret.maxX = std::max(ret.maxX, x_old); ret.minX = std::min(ret.minX, x_old);
		ret.maxY = std::max(ret.maxY, y_old); ret.minY = std::min(ret.minY, y_old);
		char c{' '};
		while(c != '\n' && c != '\0')
		{
			ifs.ignore(3); // "-> "
			int x_new{}, y_new{};
			ifs >> x_new;
			ifs.ignore(1); // ,
			ifs >> y_new;

			ret.maxX = std::max(ret.maxX, x_new); ret.minX = std::min(ret.minX, x_new);
			ret.maxY = std::max(ret.maxY, y_new); ret.minY = std::min(ret.minY, y_new);

			ret.m.emplace(x_new, y_new);
			if(y_old == y_new)
			{
				for(int x{x_old}; x != x_new; x += aocutils::sgn(x_new-x_old))
				{
					ret.m.emplace(x, y_old);
				}
			} else if(x_old == x_new)
			{
				for(int y{y_old}; y != y_new; y += aocutils::sgn(y_new-y_old))
				{
					ret.m.emplace(x_old, y);
				}
			} else
				throw std::runtime_error("no diagonal rocks supported");
			
			x_old = x_new; y_old = y_new;

			ifs.get(c); // space or newline
		}
	}
	return ret;
}

std::optional<std::pair<int,int>> fall(map_t& map, int x, int y, bool fallB)
{
	int x_orig{x}, y_orig{y};
	int floor{map.maxY + 2};
	while(true)
	{
		// check for floor before moving
		if(y >= floor-1) return {{x,y}};
		// go down
		if(!map.m.contains({x,y+1}))
		{
			y++;
			// std::cout << "down (" << x << "," << y << ")" << std::endl;
		}
		// go left down
		else if (!map.m.contains({x-1,y+1}))
		{
			y++; x--;
			// std::cout << "left down (" << x << "," << y << ")" << std::endl;
		}
		// go right down
		else if (!map.m.contains({x+1,y+1}))
		{
			y++; x++;
			// std::cout << "right down (" << x << "," << y << ")" << std::endl;
		// stuck
		} else
		{
			if(fallB && x == x_orig && y == y_orig) return {};
			return {{x,y}};
		}
		if(!fallB)
		{
			// check if there is any chance the block meets another block someday
			if(x < map.minX || x > map.maxX || y > map.maxY) return {};
		} else {
			if(x == x_orig && y == y_orig) return {};
		}
	}
}

int part(map_t map, bool fallB = false)
{
	for(int i{0}; true; i++)
	{
		auto p = fall(map, 500, 0, fallB);
		if(p.has_value())
		{
			// std::cout << "                      insert at "  << p->first << " " << p->second << std::endl;
			map.m.insert(std::move(*p));
		} else {
			return i;
		}
	}
}

int main()
{
	// std::ifstream ifs{"../problems/day14.dat.testing"};
	std::ifstream ifs{"../problems/day14.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	auto map{parse(ifs)};

	auto part1 = part(map);
	auto part2 = part(map, true) + 1; // +1 for the 500|0 block

	std::cout << "Day 14:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
