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
	std::optional<signed int> x;

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
			signed int tmp{};
			is >> tmp;
			i.x.emplace(tmp);
		} else if (instr == "noop")
		{
			i.type = NOOP;
			i.x.reset();
		} else throw std::runtime_error("Invalid instruction");

		return is;
	}
};

int partA(std::vector<Instruction> instructions)
{
	int reg{1};
	int part1{0};
	int cycle{1};
	for(const auto& i : instructions)
	{
		// std::cout << static_cast<int>(i.type) << " " << i.x.value_or(0) << std::endl;
		for(int k{0}; k < static_cast<int>(i.type); k++)
		{
			// std::cout << reg << " " << cycle << " " << (reg * cycle) << std::endl;
			if((cycle-20) % 40 == 0)
			{
				// std::cout << reg << " " << cycle << " " << (reg * cycle) << std::endl;
				part1+= (reg * cycle);
			}
			cycle++;
		}
		using enum Instr_Type;
		if(i.type == ADD) reg += *i.x;
	}
	return part1;
}

std::string partB(std::vector<Instruction> instructions)
{
	int reg{1};
	std::ostringstream part1{};
	part1 << "\n";
	int cycle{0};
	for(const auto& i : instructions)
	{
		for(int k{0}; k < static_cast<int>(i.type); k++)
		{
			// std::cout << cycle << " " << reg << std::endl;
			if(abs(reg - (cycle%40)) <= 1)
				part1 << "#";
			else
				part1 << ".";
			if((cycle+1) % 40 == 0) part1 << "\n";
			cycle++;
		}
		using enum Instr_Type;
		if(i.type == ADD) reg += *i.x;
	}
	return part1.str();
}

int main()
{
	// std::ifstream ifs{"../problems/day10.dat.testing"};
	// std::ifstream ifs{"../problems/day10.dat.testing2"};
	std::ifstream ifs{"../problems/day10.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Instruction> instructions = aocutils::vectorize_ifs<Instruction>(ifs);


	auto part1 = partA(instructions);
	auto part2 = partB(instructions);

	std::cout << "Day 10:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
