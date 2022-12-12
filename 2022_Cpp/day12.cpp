#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <ranges>
#include <vector>

#include "aocutils.h"

struct Field
{
	std::vector<int> f;
	unsigned int     src;
	unsigned int     dst;
	unsigned int     dim;

	friend std::istream &operator>>(std::istream &is, Field &i)
	{
		// default initialize the most probable moved from object
		i     = std::move(Field{});
		i.dim = 0;

		char x{};
		bool first{true};
		is.get(x);
		if(is.eof()) return is;
		while(!is.eof())
		{
			if(x != '\n')
			{
				if(first) i.dim++;
				if(x == 'S')
				{
					i.src = static_cast<unsigned int>(i.f.size());
					x     = 'a';
				}
				else if(x == 'E')
				{
					i.dst = static_cast<unsigned int>(i.f.size());
					x     = 'z';
				}
				i.f.push_back(x - 'a');
			}
			else first = false;
			is.get(x);
		}
		is.clear();
		return is;
	}
};

using idx_t = unsigned int; // = eles for dijkstra
int partA(Field &field)
{
	std::vector<int> distances(
		field.f.size(), std::numeric_limits<int>::max()); // default to inf
	auto set_distance = [&distances](idx_t &idx, int dist)
	{ distances.at(idx) = dist; };
	auto get_distance = [&distances](const idx_t &idx)
	{ return distances.at(idx); };

	std::vector<bool> visited(field.f.size(), false); // default to false
	auto              set_visited = [&visited](idx_t &idx, bool vis)
	{ visited.at(idx) = vis; };
	auto get_visited = [&visited](const idx_t &idx) { return visited.at(idx); };

	auto neighbours = [&field](const idx_t &idx)
	{
		std::vector<std::pair<idx_t, int>>
			ret{}; // idx and weight
		int cur = field.f.at(idx);

		for(const auto ele : std::array<signed long, 4>{
				idx - 1, idx + 1, idx + field.dim, idx - field.dim})
		{
			if(ele < 0 || ele >= static_cast<long>(field.f.size())) continue;
			if(field.f.at(static_cast<unsigned>(ele)) <= cur ||
			   field.f.at(static_cast<unsigned>(ele)) == cur + 1)
			{
				ret.emplace_back(ele, 1);
			}
		}
		return ret;
	}; // end neighbours

	aocutils::dijkstra<idx_t>(
		set_distance, get_distance, set_visited, get_visited,
		[](idx_t, idx_t) { ; }, neighbours, field.src);

	return get_distance(field.dst);
}

int partB(Field &field)
{
	int min = std::numeric_limits<int>::max();
	for(unsigned int i{0}; i < field.f.size(); i++)
	{
		if(field.f.at(i) != 0) continue;
		field.src = i;
		min       = std::min(min, partA(field));
	}
	return min;
}

int main()
{
	// std::ifstream ifs{"../problems/day12.dat.testing"};
	std::ifstream ifs{"../problems/day12.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	Field field{};
	ifs >> field;

	auto part1 = partA(field);
	auto part2 = partB(field);

	std::cout << "Day 12:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
