#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "aocutils.h"

struct Class
{
	int amount;
	int src, dst;

	friend std::istream &operator>>(std::istream &is, Class &i)
	{
		throw std::runtime_error("Parsing not implemented");
		// default initialize the most probable moved from object
		i = std::move( Class{});
		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, Class &i)
	{
		throw std::runtime_error("Printing not implemented");
		return os;
	}
};

int main()
{
	// std::ifstream ifs{"../problems/day00.dat.testing"};
	std::ifstream ifs{"../problems/day00.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Class> instructions = aocutils::vectorize_ifs<Class>(ifs);

	auto part1 = "dummy";
	auto part2 = "dummy";

	std::cout << "Day 00:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
