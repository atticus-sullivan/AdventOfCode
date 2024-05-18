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

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

#include "aocutils.h"
using Range = aocutils::Range<int>;

long get_prio(char x)
{
	if('a' <= x && x <= 'z') { return x - 'a' + 1; }
	else if('A' <= x && x <= 'Z') { return x - 'A' + 27; }
	throw std::runtime_error("Invalid Rucksack content");
}

struct Rucksack
{
	Range a, b;
	bool  contained;
	bool  overlapped;

	friend std::istream &operator>>(std::istream &is, Rucksack &i)
	{
		i = std::move(Rucksack{});
		char tmp;
		int  low, high;

		is >> low >> tmp >> high;
		if(is.bad() || tmp == '\0') return is;
		i.a = Range{low, high};

		is >> tmp;

		is >> low >> tmp >> high;
		if(is.bad() || tmp == '\0') return is;
		i.b = Range{low, high};

		i.contained  = i.a.contains(i.b);
		i.overlapped = i.a.overlap(i.b);

		return is;
	}
};

int main()
{
	// std::ifstream ifs{"../problems/day04.dat.testing"};
	std::ifstream ifs{"../problems/day04.dat"};
	if(ifs.fail()) { throw std::runtime_error("File couldn't be opened!"); }
	std::vector<Rucksack> rucksacks = aocutils::vectorize_ifs<Rucksack>(ifs);

	auto part1 = std::count_if(rucksacks.begin(), rucksacks.end(),
							   [](const auto &a) { return a.contained; });
	auto part2 = std::count_if(rucksacks.begin(), rucksacks.end(),
							   [](const auto &a) { return a.overlapped; });

	std::cout << "Day 04:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
