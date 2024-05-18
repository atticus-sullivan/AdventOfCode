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
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "aocutils.h"

struct Elf
{
	std::vector<int> bag;
	int              calories;

	operator int() { return calories; }

	friend std::istream &operator>>(std::istream &is, Elf &i)
	{
		i = std::move(Elf{});
		// std::cout << &i << std::endl;
		// std::cout << i.bag.size() << std::endl;
		std::string l;
		std::getline(is, l);
		while(l != "")
		{
			// std::cout << "'" << l << "'" << std::endl;
			i.bag.push_back(std::stoi(l));
			std::getline(is, l);
		}
		i.calories = std::reduce(i.bag.begin(), i.bag.end());
		return is;
	}
};

int main()
{
	// std::ifstream ifs{"../problems/day01.dat.testing"};
	std::ifstream ifs{"../problems/day01.dat"};
	if(ifs.fail()) { throw std::runtime_error("File couldn't be opened!"); }
	std::vector<Elf> elves = aocutils::vectorize_ifs<Elf>(ifs);

	std::sort(elves.begin(), elves.end(),
			  [](const auto &a, const auto &b)
			  { return a.calories < b.calories; });
	auto part2 = std::accumulate(elves.end() - 3, elves.end(), 0);
	auto part1 = (elves.end() - 1)->calories;
	// old stuff before the need of sorting the vector
	// auto x = std::ranges::max_element(elves.begin(), elves.end(), [](const
	// auto& a, const auto& b) {return a.calories < b.calories;});

	std::cout << "Day 01:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
