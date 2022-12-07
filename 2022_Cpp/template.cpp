#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <cassert>

#include "aocutils.h"

struct Class {
	int amount;
	int src, dst;

	friend std::istream& operator>>(std::istream& is, Class& i) {
		throw std::runtime_error("Parsing not implemented");
		i = std::move(Class{}); // default initialize the most probable moved from object
		return is;
	}
};

int main(int argc, char* argv[]) {
	// std::ifstream ifs{"../problems/day00.dat.testing"};
	std::ifstream ifs{"../problems/day00.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Class> instructions = aocutils::vectorize_ifs<Class>(ifs);

	std::string part1 = "dummy";
	std::string part2 = "dummy";

	std::cout << "Day 04:" << '\n'
		<< "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << part2 << std::endl
		;
}
