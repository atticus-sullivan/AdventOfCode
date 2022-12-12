#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <vector>
#include <functional>

#include "aocutils.h"

// assumes that distance is alrwady initialized with infinity
template <typename T>
void dijkstra(
		std::function<void(T,int)> set_distance, std::function<int(T)> get_distance,
		std::function<void(T, bool)> set_visited, std::function<bool(T)> get_visited,
		std::function<std::vector<std::pair<T,int>>(T)> neighbours,
		std::function<void(T,T)> set_parent,
		T start)
{
	auto comp = [](const std::pair<int,T>& a, const std::pair<int,T>& b){return a.first > b.first;};
	std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T>>, decltype(comp)> pq{};
	pq.emplace(0,start);
	while(pq.size() > 0)
	{
		auto [_,v] = pq.top();
		pq.pop();
		// check if v is already processed (because of ust inserting new values
		// to the pq instead of dooing an uüdate)
		// std::cout << "Selected " << v << std::endl;
		if(get_visited(v)) continue;
		set_visited(v, true);

		for(auto& [w,c] : neighbours(v))
		{
			// std::cout << "w = " << w << std::endl;
			int dist = get_distance(v) + c;
			if(dist < get_distance(w))
			{
				set_parent(w, v);
				pq.push(std::make_pair(dist,w));
				set_distance(w, dist);
			}
		}
		// std::cout << "Finished " << v << std::endl;
	}
}

struct Field
{
	std::vector<int> f;
	unsigned int src;
	unsigned int dst;
	unsigned long dim;

	friend std::istream &operator>>(std::istream &is, Field &i)
	{
		// default initialize the most probable moved from object
		i = std::move( Field{});
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
				if(x == 'S')      { i.src = i.f.size(); x = 'a'; }
				else if(x == 'E') { i.dst = i.f.size(); x = 'z'; }
				i.f.push_back(x - 'a');
			}
			else
				first = false;
			is.get(x);
		}
		is.clear();
		return is;
	}
	// friend std::ostream &operator<<(std::ostream &os, Field &i)
	// {
	// 	throw std::runtime_error("Printing not implemented");
	// 	return os;
	// }
};

int partA(Field& field)
{
	std::vector<int> distances(field.f.size(), 10000);
	distances.at(field.src) = 0;
	auto set_distance = [&distances](unsigned int idx,int dist) { distances.at(idx) = dist; };
	auto get_distance = [&distances](unsigned int idx) { return distances.at(idx); };

	std::vector<bool> visited(field.f.size(), false);
	auto set_visited = [&visited](unsigned int idx,int dist) { visited.at(idx) = dist; };
	auto get_visited = [&visited](unsigned int idx) { return visited.at(idx); };

	auto neighbours = [&field](int idx)
	{
		std::vector<std::pair<unsigned long, int>> ret{};
		int cur = field.f.at(idx);

		for(const auto ele : std::array{idx-1, idx+1, idx+static_cast<int>(field.dim), idx-static_cast<int>(field.dim)})
		{
			if(ele < 0 || ele >= static_cast<long>(field.f.size())) continue;
			if(field.f.at(ele) <= cur || field.f.at(ele) == cur+1)
			{
				ret.emplace_back(ele, 1);
			}
		}

		return ret;
	};

	dijkstra<unsigned long>(set_distance, get_distance, set_visited, get_visited, neighbours, [](int,int){;}, field.src);

	return get_distance(field.dst);
}

int partB(Field& field)
{
	int min = 10000;
	for(unsigned int i{0}; i < field.f.size(); i++)
	{
		if(field.f.at(i) != 0) continue;
		field.src = i;
		min = std::min(min, partA(field));
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
			  << "  Part 2: " << part2 << std::endl
			  ;
}
