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
using Range = aocutils::Range<int>;

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
	Range a, b;
	bool contained;
	bool overlapped;

	friend std::istream& operator>>(std::istream& is, Rucksack& i) {
		i = std::move(Rucksack{});
		char tmp;
		int low, high;

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

int main(int argc, char* argv[]) {
	// std::ifstream ifs{"../day04.dat.testing"};
	std::ifstream ifs{"../day04.dat"};
	if(ifs.fail()){
		throw std::runtime_error("File couldn't be opened!");
	}
	std::vector<Rucksack> rucksacks = aocutils::vectorize_ifs<Rucksack>(ifs);

	auto part1 = std::count_if(rucksacks.begin(), rucksacks.end(), [](const auto& a){return a.contained;});
	auto part2 = std::count_if(rucksacks.begin(), rucksacks.end(), [](const auto& a){return a.overlapped;});

	std::cout << "Day 04:" << '\n'
		<< "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << part2 << std::endl
		;
}
