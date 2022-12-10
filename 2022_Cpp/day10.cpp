#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <vector>
#include <sstream>

#include "aocutils.h"

enum class Instr_Type
{
	ADD=2,
	NOOP=1,
};

struct Instruction
{
	Instr_Type type;
	signed int x;

	friend std::istream &operator>>(std::istream &is, Instruction &i)
	{
		using enum Instr_Type;
		std::string instr{};
		is >> instr;
		if (is.bad() || instr == "") return is;
		// std::cout << instr << std::endl;
		if (instr == "addx")
		{
			i.type = ADD;
			is >> i.x;
		} else if (instr == "noop")
		{
			i.type = NOOP;
			// just handle noop like add with x=0 in respect of modifying the
			// register
			i.x = 0;
		} else throw std::runtime_error("Invalid instruction");

		return is;
	}
};

int main()
{
	using enum Instr_Type;
	// std::ifstream ifs{"../problems/day10.dat.testing"};
	// std::ifstream ifs{"../problems/day10.dat.testing2"};
	std::ifstream ifs{"../problems/day10.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	// only read input once and simply use an iterator for the loop (no storing
	// in a vector)
	auto instructions = std::views::istream<Instruction>(ifs);

	int                reg{1};
	int                cycle{1};
	std::ostringstream part2_{};
	part2_ << "\n";
	int part1{0};
	for(const auto &i : instructions)
	{
		for(int k{0}; k < static_cast<int>(i.type); k++)
		{
			// part1
			if((cycle - 20) % 40 == 0) { part1 += (reg * cycle); }
			// part2
			if(abs(reg - ((cycle - 1) % 40)) <= 1) part2_ << "#";
			else part2_ << ".";
			if(cycle % 40 == 0) part2_ << "\n";
			cycle++;
		}
		reg += i.x;
	}

	auto part2 = part2_.str();

	std::cout << "Day 10:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
