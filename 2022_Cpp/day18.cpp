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
#include <forward_list>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "aocutils.h"

struct Cube
{
	int x;
	int y;
	int z;

	friend constexpr bool operator<(const Cube &lhs, const Cube &rhs)
	{
		return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
	}
	friend constexpr bool operator==(const Cube &lhs, const Cube &rhs)
	{
		return std::tie(lhs.x, lhs.y, lhs.z) == std::tie(rhs.x, rhs.y, rhs.z);
	}

	friend std::istream &operator>>(std::istream &is, Cube &i)
	{
		// default initialize the most probable moved from object
		i = std::move(Cube{});
		is >> i.x;
		is.ignore(1);
		is >> i.y;
		is.ignore(1);
		is >> i.z;
		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, const Cube &i)
	{
		os << i.x << "," << i.y << "," << i.z;
		return os;
	}
};

uint flood_fill(std::set<Cube> &cubes, std::set<Cube> &visited, Cube ref,
				const Cube &last, const Cube &max)
{
	if(visited.contains(ref)) { return 0; }
	visited.insert(ref);
	if(ref.x < -1 || ref.y < -1 || ref.z < -1) { return 0; }
	if(ref.x > max.x || ref.y > max.y || ref.z > max.z) { return 0; }
	// std::cout << "ref:" << ref << std::endl;

	uint cnt{0};
	Cube current = ref;

	current.x += 1;
	if(current != last)
	{
		if(cubes.contains(current)) { cnt++; }
		else { cnt += flood_fill(cubes, visited, current, ref, max); }
	}

	current.x -= 2;
	if(current != last)
	{
		if(cubes.contains(current)) { cnt++; }
		else { cnt += flood_fill(cubes, visited, current, ref, max); }
	}

	current.x += 1;
	current.y += 1;
	if(current != last)
	{
		if(cubes.contains(current)) { cnt++; }
		else { cnt += flood_fill(cubes, visited, current, ref, max); }
	}

	current.y -= 2;
	if(current != last)
	{
		if(cubes.contains(current)) { cnt++; }
		else { cnt += flood_fill(cubes, visited, current, ref, max); }
	}

	current.y += 1;
	current.z += 1;
	if(current != last)
	{
		if(cubes.contains(current)) { cnt++; }
		else { cnt += flood_fill(cubes, visited, current, ref, max); }
	}

	current.z -= 2;
	if(current != last)
	{
		if(cubes.contains(current)) { cnt++; }
		else { cnt += flood_fill(cubes, visited, current, ref, max); }
	}

	return cnt;
}

uint count(std::forward_list<Cube> &cubes, std::map<Cube, bool> &visited,
		   Cube c)
{
	uint cnt{0};
	cubes.remove(c);
	visited[c] = true;

	// check adjacent sides, if cube is connected -> count the surface of the
	// adjacent cube, else increase the count of exposed sides
	Cube ref{.x = c.x, .y = c.y, .z = c.z};
	ref.x += 1;
	if(visited.contains(ref))
	{
		if(!visited[ref]) { cnt += count(cubes, visited, ref); }
	}
	else { cnt++; }

	ref.x -= 2;
	if(visited.contains(ref))
	{
		if(!visited[ref]) { cnt += count(cubes, visited, ref); }
	}
	else { cnt++; }

	ref.x = c.x;
	ref.y += 1;
	if(visited.contains(ref))
	{
		if(!visited[ref]) { cnt += count(cubes, visited, ref); }
	}
	else { cnt++; }

	ref.y -= 2;
	if(visited.contains(ref))
	{
		if(!visited[ref]) { cnt += count(cubes, visited, ref); }
	}
	else { cnt++; }

	ref.y = c.y;
	ref.z += 1;
	if(visited.contains(ref))
	{
		if(!visited[ref]) { cnt += count(cubes, visited, ref); }
	}
	else { cnt++; }

	ref.z -= 2;
	if(visited.contains(ref))
	{
		if(!visited[ref]) { cnt += count(cubes, visited, ref); }
	}
	else { cnt++; }

	return cnt;
}

uint solve_part1(const std::vector<Cube> &cubes_vec)
{
	std::forward_list<Cube> cubes{cubes_vec.begin(), cubes_vec.end()};

	// init cubes as not visited
	std::map<Cube, bool> visited;
	for(auto c : cubes) { visited.emplace(c, false); }

	uint cnt{0};
	while(!cubes.empty()) { cnt += count(cubes, visited, cubes.front()); }
	return cnt;
}

uint solve_part2(std::vector<Cube> &cubes)
{
	auto maxX =
		std::ranges::max(cubes, std::less<>{}, [](Cube &c) { return c.x; }).x +
		1;
	auto maxY =
		std::ranges::max(cubes, std::less<>{}, [](Cube &c) { return c.y; }).y +
		1;
	auto maxZ =
		std::ranges::max(cubes, std::less<>{}, [](Cube &c) { return c.z; }).z +
		1;
	auto           cubes_set = std::set(cubes.begin(), cubes.end());
	std::set<Cube> vis{};
	auto           max = Cube{.x = maxX, .y = maxY, .z = maxZ};
	return flood_fill(cubes_set, vis, Cube{.x = maxX, .y = maxY, .z = maxZ},
					  Cube{.x = maxX, .y = maxY, .z = maxZ}, max);
}

int main()
{
	// std::ifstream ifs{"../problems/day18.dat.testing"};
	std::ifstream ifs{"../problems/day18.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Cube> cubes = aocutils::vectorize_ifs<Cube>(ifs);

	auto part1 = solve_part1(cubes);
	auto part2 = solve_part2(cubes);

	std::cout << "Day 18:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
