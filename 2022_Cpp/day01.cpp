#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <numeric>

#include "aocutils.h"

struct Elf {
	std::vector<int> bag;
	int calories;

	operator int(){return calories;}

	friend std::istream& operator>>(std::istream& is, Elf& i) {
		i = std::move(Elf{});
		// std::cout << &i << std::endl;
		// std::cout << i.bag.size() << std::endl;
		std::string l;
		std::getline(is, l);
		while(l != ""){
			// std::cout << "'" << l << "'" << std::endl;
			i.bag.push_back(std::stoi(l));
			std::getline(is, l);
		}
		i.calories = std::reduce(i.bag.begin(), i.bag.end());
		return is;
	}
};

int main(int argc, char* argv[]) {
	// std::ifstream ifs{"../problems/day01.dat.testing"};
	std::ifstream ifs{"../problems/day01.dat"};
	if(ifs.fail()){
		throw std::runtime_error("File couldn't be opened!");
	}
	std::vector<Elf> elves = aocutils::vectorize_ifs<Elf>(ifs);

	std::sort(elves.begin(), elves.end(), [](const auto& a, const auto& b) {return a.calories < b.calories;});
	auto part2 = std::accumulate(elves.end()-3, elves.end(), 0);
	auto part1 = (elves.end()-1)->calories;
	// old stuff before the need of sorting the vector
	// auto x = std::ranges::max_element(elves.begin(), elves.end(), [](const auto& a, const auto& b) {return a.calories < b.calories;});

	std::cout << "Day 01:" << '\n'
		<< "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << part2 << std::endl
		;
}
