#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <set>
#include <numeric>
#include <cassert>

#include "aocutils.h"

long get_prio(char x){
	if('a' <= x && x <= 'z'){
		return x - 'a' + 1;
	}
	else if('A' <= x && x <= 'Z'){
		return x - 'A' + 27;
	}
	throw std::runtime_error("Invalid Rucksack content");
}

struct Rucksack {
	std::set<char> compartmentA;
	std::set<char> compartmentB;
	std::set<char> all;

	std::set<char> common;
	long prioA;

	friend std::istream& operator>>(std::istream& is, Rucksack& i) {
		i = std::move(Rucksack{});
		std::string l;
		std::getline(is, l);
		if(l == "") return is;

		i.all = std::set<char>{l.begin(), l.end()};

		i.compartmentA = std::set<char>{l.begin(), l.begin()+l.length()/2};
		// for(auto const& ele : i.compartmentA){
		// 	std::cout << ele << " ";
		// }
		// std::cout << std::endl;

		i.compartmentB = std::set<char>{l.begin()+l.length()/2, l.end()};
		// for(auto const& ele : i.compartmentB){
		// 	std::cout << ele << " ";
		// }
		// std::cout << std::endl;

		// std::cout << i.a.size() << " " << i.b.size() << " (" << l << ")" << std::endl;
		// assert(i.a.size() == i.b.size() && "a.size() != b.size()");

		i.common = std::set<char>{};

		std::set_intersection(i.compartmentA.begin(), i.compartmentA.end(), i.compartmentB.begin(), i.compartmentB.end(), std::inserter(i.common, i.common.begin()));
		i.prioA = 0;
		for(auto const& ele : i.common){
			// std::cout << "'" << ele << "' ";
			i.prioA += get_prio(ele);
		}
		// std::cout << std::endl;
		assert(i.common.size() == 1 && "Not exactly one element in common");

		return is;
	}
};

int main(int argc, char* argv[]) {
	// std::ifstream ifs{"../problems/day03.dat.testing"};
	std::ifstream ifs{"../problems/day03.dat"};
	if(ifs.fail()){
		throw std::runtime_error("File couldn't be opened!");
	}
	std::vector<Rucksack> rucksacks = aocutils::vectorize_ifs<Rucksack>(ifs);

	auto part1 = std::accumulate(rucksacks.begin(), rucksacks.end(), 0, [](long b, const auto& r){return r.prioA+b;});

	long part2 = 0;
	std::set<char> tmpA{};
	std::set<char> tmpB{};
	for(auto it=rucksacks.begin(); it != rucksacks.end() && it+1 != rucksacks.end() && it+2 != rucksacks.end();){
		tmpA.clear();
		tmpB.clear();
		std::set_intersection(it->all.begin(), it->all.end(), (it+1)->all.begin(), (it+1)->all.end(), std::inserter(tmpA, tmpA.begin()));
		std::set_intersection(tmpA.begin(), tmpA.end(), (it+2)->all.begin(), (it+2)->all.end(), std::inserter(tmpB, tmpB.begin()));
		for(const auto& ele : tmpB){
			// std::cout << "'" << ele << "' ";
			part2 += get_prio(ele);
		}
		// std::cout << std::endl;
		assert(tmpB.size() == 1 && "Not exactly one element in common");
		it += 3;
	}
	// auto part2 = std::accumulate(games.begin(), games.end(), 0, [](int b, const auto& r){return r.pointsB+b;});

	std::cout << "Day 03:" << '\n'
		<< "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << part2 << std::endl
		;
}
