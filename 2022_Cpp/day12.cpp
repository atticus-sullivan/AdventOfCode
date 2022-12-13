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
int part(Field &field, std::function<std::vector<std::pair<idx_t,int>>(const idx_t&)> neighbours, std::function<bool(const idx_t&)> pred, idx_t& start)
{
	std::vector<idx_t> parents(
		field.f.size(), std::numeric_limits<int>::max()); // default to inf -> invalid value
	auto set_parent = [&parents](idx_t &idx, const idx_t &parent)
	{ parents.at(idx) = parent; };

	std::vector<bool> visited(field.f.size(), false); // default to false
	auto              set_visited = [&visited](idx_t &idx, bool vis)
	{ visited.at(idx) = vis; };
	auto get_visited = [&visited](const idx_t &idx) { return visited.at(idx); };

	auto dst = aocutils::breath_first_search<idx_t>(
		set_visited, get_visited,
		set_parent, neighbours,
		pred,
		start);

	int length{0};

	if(!dst.has_value()) return std::numeric_limits<int>::max();
	for(idx_t cur{*dst}; cur != start;)
	{
		length++;
		cur = parents.at(cur);
		if(cur >= parents.size()) return std::numeric_limits<int>::max();
	}

	return length;
}
int partA(Field &field)
{
	auto neighbours = [&field](const idx_t &idx)
	{
		std::vector<std::pair<idx_t, int>>
			ret{}; // idx and weight
		int cur = field.f.at(idx);

		for(const auto ele : std::array{
				idx - 1, idx + 1, idx + field.dim, idx - field.dim})
		{
			if(ele >= field.f.size()) continue;
			// std::cout << "neigh: " << ele << std::endl;
			if(field.f.at(static_cast<unsigned>(ele)) <= cur ||
			   field.f.at(static_cast<unsigned>(ele)) == cur + 1)
			{
				ret.emplace_back(ele, 1);
			}
		}
		return ret;
	}; // end neighbours

	auto pred = [&field](const idx_t v){return v == field.dst;};


	return part(field, neighbours, pred, field.src);
}

int partB(Field &field)
{
	auto neighbours = [&field](const idx_t &idx)
	{
		std::vector<std::pair<idx_t, int>>
			ret{}; // idx and weight
		int cur = field.f.at(idx);

		for(const auto ele : std::array{
				idx - 1, idx + 1, idx + field.dim, idx - field.dim})
		{
			if(ele >= field.f.size()) continue;
			// std::cout << "neigh: " << ele << std::endl;
			if(field.f.at(static_cast<unsigned>(ele)) >= cur ||
			   field.f.at(static_cast<unsigned>(ele)) == cur - 1)
			{
				ret.emplace_back(ele, 1);
			}
		}
		return ret;
	}; // end neighbours

	auto pred = [&field](const idx_t v){return field.f.at(v) == field.f.at(field.src);};

	// do the BFS just as in partA but with stuff reversed
	// works since BFS always finds the nearest node according to `pred` (at
	// least in an unweighted graph)
	return part(field, neighbours, pred, field.dst);
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
			  << "  Part 2: " << part2 << std::endl
			  ;
}
