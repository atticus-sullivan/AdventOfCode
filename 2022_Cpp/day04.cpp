#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <set>
#include <numeric>
#include <cassert>

long get_prio(char x){
	if('a' <= x && x <= 'z'){
		return x - 'a' + 1;
	}
	else if('A' <= x && x <= 'Z'){
		return x - 'A' + 27;
	}
	throw std::runtime_error("Invalid Rucksack content");
}

struct Range{
	int low, high;

	Range(): low{0}, high{0} {}
	Range(int low, int high): low{low}, high{high} {
		assert(low <= high && "low <= high");
	}

	bool contains(Range other){
		return
			(low <= other.low && high >= other.high) ||
			(other.low <= low && other.high >= high);
	}
	bool overlap(Range other){
		bool r =
			(low <= other.low && high < other.low) ||
			(other.low <= low && other.high < low);
		return !r;
	}
};

bool contains(int lowA, int highA, int lowB, int highB){
	return
		(lowA <= lowB && highA >= highB) ||
		(lowB <= lowA && highB >= highA);
}
bool overlap(int lowA, int highA, int lowB, int highB){
	bool r =
		(lowA < lowB && highA < lowB) ||
		(lowB < lowA && highB < lowA);
	return !r;
}

struct Rucksack {
	Range a, b;
	bool contained;
	bool overlapped;

	friend std::istream& operator>>(std::istream& is, Rucksack& i) {
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
	std::vector<Rucksack> rucksacks{std::istream_iterator<Rucksack>{ifs}, {}};

	auto part1 = std::count_if(rucksacks.begin(), rucksacks.end(), [](const auto& a){return a.contained;});
	auto part2 = std::count_if(rucksacks.begin(), rucksacks.end(), [](const auto& a){return a.overlapped;});

	std::cout << "Day 04:" << '\n'
		<< "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << part2 << std::endl
		;
}
