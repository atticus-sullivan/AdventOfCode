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
#include <map>
#include <vector>

#include "aocutils.h"

struct Class
{
	int amount;
	int src, dst;

	friend std::istream &operator>>(std::istream &is, Class &i)
	{
		throw std::runtime_error("Parsing not implemented");
		// default initialize the most probable moved from object
		i = std::move( Class{});
		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, Class &i)
	{
		throw std::runtime_error("Printing not implemented");
		return os;
	}
};

int main()
{
	// std::ifstream ifs{"../problems/day00.dat.testing"};
	std::ifstream ifs{"../problems/day00.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Class> instructions = aocutils::vectorize_ifs<Class>(ifs);

	auto part1 = "dummy";
	auto part2 = "dummy";

	std::cout << "Day 00:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
