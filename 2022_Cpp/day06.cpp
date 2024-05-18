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
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "aocutils.h"

struct Message
{
	std::string msg;

	int search_start(unsigned int count)
	{
		auto left{msg.begin()};
		auto right{msg.begin()};
		// storee how often which element occurs
		std::map<char, int> eleMap{};

		unsigned int j{0}; // how many characters are being read so far
		// initialize with the first count characters
		for(; j < count; j++, right++) { eleMap[*right]++; }
		// shift window one char to the right each time
		for(; right != msg.end(); right++, left++, j++)
		{
			// count distict elements contained in Map
			if(eleMap.size() == count) return static_cast<int>(j);
			// left char occurs one time less in range, check if it is to be
			// removed
			if(--eleMap[*left] == 0) eleMap.erase(*left);
			// right character occurs one time more in range
			eleMap[*right]++;
		}
		return -1;
	}

	friend std::istream &operator>>(std::istream &is, Message &i)
	{
		// default initialize the most probable moved from object
		i = std::move(Message{});

		std::getline(is, i.msg);
		return is;
	}
};

int main()
{
	// std::ifstream ifs{"../problems/day06.dat.testing"};
	std::ifstream ifs{"../problems/day06.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	Message m{};
	ifs >> m;

	int part1 = m.search_start(4);
	int part2 = m.search_start(14);

	std::cout << "Day 04:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
