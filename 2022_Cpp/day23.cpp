#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "aocutils.h"

using Pos = std::pair<int,int>;
using Map = std::set<Pos>;
using Plan = std::pair<std::map<Pos,Pos>, std::map<Pos,int>>;
using Dir = std::pair<int,int>;

struct Order {
	std::vector<Dir> check_before;
	Dir move;

	Pos check(const Map& map, const Pos& p) const {
		for(const auto& c : check_before) {
			// if(p == Pos{3,-3}) {
			// 	std::cout << "check " << (p.first+c.first) << "|" << (p.second+c.second) << std::endl;
			// }
			if(map.contains({p.first+c.first, p.second+c.second})) {
				return p;
			}
		}
		return {p.first+move.first, p.second+move.second};
	}
};

Map parse(std::istream &is) {
	is >> std::noskipws;
	char c{};
	int y{0}, x{0};
	Map ret{};
	while(!is.eof()) {
		is >> c;
		if(is.eof())
			break;
		if(c == '\n') {
			x = 0;
			y--;
			continue;
		}
		if(c == '#') {
			ret.insert({x,y});
		}
		x++;
	}
	return ret;
}

void print_map(Map &map) {
	auto mima_x = std::minmax_element(map.begin(), map.end(), [](Pos a, Pos b){return a.first < b.first;});
	auto mima_y = std::minmax_element(map.begin(), map.end(), [](Pos a, Pos b){return a.second < b.second;});

	for(auto y{mima_y.second->second}; y >= mima_y.first->second; y--) {
		for(auto x{mima_x.first->first}; x <= mima_x.second->first; x++) {
			if(map.contains({x,y}))
				std::cout << '#';
			else
				std::cout << '.';
		}
		std::cout << std::endl;
	}
}

Plan t(const Map& map, std::vector<Order>::const_iterator cur_order, const std::vector<Order>& order) {
	Plan ret{};

	for(const auto& [x,y] : map) {
		// check if another elve is around
		bool found{false};
		for(int cx{x-1}; cx <= x+1; cx++) {
			for(int cy{y-1}; cy <= y+1; cy++) {
				if(cx == x && cy == y) continue;
				if(map.contains({cx,cy})) {
					found = true;
					break;
				}
			}
			if(found) break;
		}
		if(!found){
			// std::cout << x << "|" << y << " no adjacent found" << std::endl;
			ret.first[{x,y}] = {x,y};
			ret.second[{x,y}]++;
			continue;
		}
		// loop in order and propose steps
		bool moved{false};
		auto o = cur_order;
		for(size_t i{0}; i < order.size(); i++, o=aocutils::advance_circle(order, o, 1)) {
			auto c{o->check(map, {x,y})};
			if(c != Pos{x,y}) {
				moved = true;
				// std::cout << x << "|" << y << " proposes " << c.first << "|" << c.second << std::endl;
				ret.first[{x,y}] = c;
				ret.second[c]++;
				break;
			}
		}
		if(!moved) {
			// std::cout << x << "|" << y << " all checks failed" << std::endl;
			ret.first[{x,y}] = {x,y};
			ret.second[{x,y}]++;
		}
	}

	return ret;
}

std::pair<int,int> solve_a(Map map, const std::vector<Order>& order, int max, bool part2) {
	auto cur_order{order.begin()};

	auto i{0};
	for(; i < max || part2; i++, cur_order = aocutils::advance_circle(order, cur_order, 1)) {
		// std::cout << std::endl;
		auto plan{t(map, cur_order, order)};
		std::set<Pos> moved{}, stayed{};
		uint real_move = 0;
		for(const auto& e : map) {
			auto n{plan.first[e]};
			if(plan.second[n] == 1) {
				if(n != e) real_move++;
				moved.insert(n);
			} else {
				stayed.insert(e);
			}
		}
		map = stayed;
		for(const auto& m: moved) {
			map.insert(m);
		}

		// std::cout << i << " -> " << real_move << std::endl;
		if(real_move == 0) break;

		// std::cout << moved.size() << "|" << stayed.size() << "|" << plan.second.size() << "|" << map.size() << std::endl;
		// print_map(map);
	}

	// print_map(map);
	auto mima_x = std::minmax_element(map.begin(), map.end(), [](Pos a, Pos b){return a.first < b.first;});
	auto mima_y = std::minmax_element(map.begin(), map.end(), [](Pos a, Pos b){return a.second < b.second;});

	// std::cout << mima_x.second->first << " - " << mima_x.first->first << std::endl;
	// std::cout << mima_y.second->second << " - " << mima_y.first->second << std::endl;
	return {(mima_x.second->first - mima_x.first->first+1) * (mima_y.second->second - mima_y.first->second+1) - static_cast<int>(map.size()), i+1};
}

int main()
{
	// std::ifstream ifs{"../problems/day23.dat.testing"};
	std::ifstream ifs{"../problems/day23.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	auto map = parse(ifs);
	// print_map(map);

	auto part1 = solve_a(map, std::vector<Order>({
		Order({{{+0,+1},{+1,+1},{-1,+1}}, {+0,+1}}),
		Order({{{+0,-1},{+1,-1},{-1,-1}}, {+0,-1}}),

		Order({{{-1,+0},{-1,+1},{-1,-1}}, {-1,+0}}),
		Order({{{+1,+0},{+1,+1},{+1,-1}}, {+1,+0}}),
	}), 10, false).first;

	auto part2 = solve_a(map, std::vector<Order>({
		Order({{{+0,+1},{+1,+1},{-1,+1}}, {+0,+1}}),
		Order({{{+0,-1},{+1,-1},{-1,-1}}, {+0,-1}}),

		Order({{{-1,+0},{-1,+1},{-1,-1}}, {-1,+0}}),
		Order({{{+1,+0},{+1,+1},{+1,-1}}, {+1,+0}}),
	}), 23, true).second;

	std::cout << "Day 23:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
