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
#include <set>
#include <vector>
#include <limits>

#include "aocutils.h"

using map_t = std::map<std::string, std::pair<int,std::map<std::string,int>>>;

auto parse(std::ifstream& ifs)
{
	map_t ret{};
	while(!ifs.eof())
	{
		std::string name;
		ifs.ignore(6); // "Valve "
		ifs >> name;

		if(ifs.eof()) break;

		int flow;
		ifs.ignore(15); // " has flow rate="
		ifs >> flow;

		ifs.ignore(18); // "; tunnels lead to "
		ifs.ignore(std::numeric_limits<std::streamsize>::max(), ' '); // "valve(s) "

		std::map<std::string,int> neighbours{};

		char c{' '};
		while(c != '\n' && c != '\0')
		{
			std::string node{};
			ifs >> node;
			if(node[node.length()-1] == ',') node.pop_back();
			neighbours.emplace(node, 1);
			ifs.get(c); // space or newline
		}

		ret.emplace(name, std::make_pair(flow, neighbours));
	}
	return ret;
}

void print(const map_t& nodes)
{
	for(auto const& [k,v] : nodes) {
		std::cout << k << " flow: " << v.first << " {";
		for(auto const& [n,d] : v.second) {
			std::cout << n << "|" << d << " ";
		}
		std::cout << "}" << std::endl;
	}
}

int solveA(map_t& nodes, std::string node = "AA",  int time_rem = 30, int pressure = 0, std::set<std::string> open = {})
{
	int ret = pressure;
	for(const auto& [n,d] : nodes.at(node).second) {
		if(d > time_rem) continue;
		if(open.contains(n)) continue;

		// open vault
		open.insert(n);
		ret = std::max(ret, solveA(nodes, n, time_rem - d, pressure + (time_rem-d)*nodes[n].first, open));
		open.erase(n);
	}

	return ret;
}

void solveB(map_t& nodes, std::map<std::set<std::string>, int>& eles, std::string node = "AA",  int time_rem = 26, int pressure = 0, std::set<std::string> open = {})
{
	if(eles.contains(open)){
		eles[open] = std::max(eles[open], pressure);
	} else {
		eles[open] = pressure;
	}
	for(const auto& [n,d] : nodes.at(node).second) {
		if(d > time_rem) continue;
		if(open.contains(n)) continue;

		// open vault
		open.insert(n);
		solveB(nodes, eles, n, time_rem - d, pressure + (time_rem-d)*nodes[n].first, open);
		open.erase(n);
	}
}

map_t dijkstra_clean_zero(const map_t& nodes)
{
	map_t nodes_new{};
	for(auto const& [k,v] : nodes) {
		if(v.first <= 0 && k != "AA") continue;

		std::map<std::string,std::string> parents{};
		std::map<std::string,int> distance{};
		std::map<std::string,bool> visited{};

		auto set_distance = [&distance](std::string &idx, int dist)
		{ distance[idx] = dist; };

		auto get_distance = [&distance](const std::string &idx)
		{
			if(distance.contains(idx))
				return distance.at(idx);
			return std::numeric_limits<int>::max();
		};

		auto set_visited = [&visited](std::string &idx, bool vis)
		{ visited[idx] = vis; };

		auto get_visited = [&visited](const std::string &idx)
		{
			if(visited.contains(idx))
				return visited.at(idx);
			return false;
		};

		auto set_parent = [&parents](std::string &idx, const std::string &parent)
		{ parents[idx] = parent; };

		auto neighbours = [&nodes](const std::string &idx)
		{
			std::vector<std::pair<std::string,int>> ret{};
			if(!nodes.contains(idx))
				return ret;
			for(auto const& [n,d] : nodes.at(idx).second) {
				ret.emplace_back(n,d);
			}
			return ret;
		};

		std::string key{k};

		// apply dijkstra
		aocutils::dijkstra<std::string>(
				set_distance, get_distance,
				set_visited, get_visited,
				set_parent,
				neighbours,
				key);

		std::map<std::string,int> neighbours_new{};
		for(const auto& [k,n] : nodes) {
			if(n.first <= 0) continue;
			neighbours_new[k] = distance[k] + 1;
		}
		nodes_new[k] = std::make_pair(v.first, std::move(neighbours_new));
	}
	return nodes_new;
}

int main()
{
	std::ifstream ifs{"../problems/day16.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	auto nodes = parse(ifs);
	// print(nodes);

	auto nodes_new = dijkstra_clean_zero(nodes);
	// print(nodes_new);

	auto part1 = solveA(nodes_new);

	std::map<std::set<std::string>, int> b{};
	solveB(nodes_new, b);

	// solution is inspired by https://github.com/juanplopes/advent-of-code-2022/blob/main/day16.py
	auto part2 = 0;
	for(const auto& [k1,v1] : b) {
		for(const auto& [k2,v2] : b) {
			if(aocutils::is_disjoint(k1, k2)){
				part2 = std::max(part2, v1 + v2);
			}
		}
	}

	std::cout << "Day 16:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
