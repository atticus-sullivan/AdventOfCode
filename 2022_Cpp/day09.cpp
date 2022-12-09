#include <cassert>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>

#include "aocutils.h"

enum class Direction
{
	RIGHT,
	UP,
	DOWN,
	LEFT,
};
std::ostream &operator<<(std::ostream &os, const Direction &i)
{
	using enum Direction;
	switch(i)
	{
		case RIGHT:
			os << "right";
			break;
		case LEFT:
			os << "left";
			break;
		case UP:
			os << "up";
			break;
		case DOWN:
			os << "down";
			break;
		default: throw std::runtime_error("Invalid direction");
	}
	return os;
}
std::istream &operator>>(std::istream &is, Direction &i)
{
	using enum Direction;
	char c{};
	is >> c;
	if(is.bad() || c == '\0') return is;
	switch(c)
	{
		case 'R': i = RIGHT; break;
		case 'U': i = UP; break;
		case 'D': i = DOWN; break;
		case 'L': i = LEFT; break;
		default: throw std::runtime_error("Unable to parse invalid direction");
	}
	return is;
}

struct Instruction
{
	int amount;
	Direction dir;

	std::tuple<std::array<int,2>,std::vector<std::array<int,2>>> exec(std::array<int,2> head, std::vector<std::array<int,2>> tail, std::set<std::array<int,2>>& track) const
	{
		using enum Direction;
		std::cout << dir << " " << amount << std::endl;
		for(int i{0}; i < amount; i++)
		{
			switch(dir)
			{
				case RIGHT:
					head[0]++;
					break;
				case LEFT:
					head[0]--;
					break;
				case UP:
					head[1]--;
					break;
				case DOWN:
					head[1]++;
					break;
				default: throw std::runtime_error("Invalid direction");
			}
			std::array<int,2>* last = &head;
			for(auto& t : tail){
				if(t[0] == (*last)[0])
				{
					// same column
					if(abs((*last)[1] - t[1]) > 1) t[1] += aocutils::sgn((*last)[1] - t[1]);
				} else if(t[1] == (*last)[1]) {
					// same row
					if(abs((*last)[0] - t[0]) > 1) t[0] += aocutils::sgn((*last)[0] - t[0]);
				} else if((abs((*last)[0] - t[0]) >= 1 && abs((*last)[1] - t[1]) > 1) || (abs((*last)[0] - t[0]) > 1 && abs((*last)[1] - t[1]) >= 1)){
					// diagonal
					t[0] += aocutils::sgn((*last)[0] - t[0]);
					t[1] += aocutils::sgn((*last)[1] - t[1]);
				}
				last = &t;
			}
			track.emplace(tail.back());
		}
		return std::make_tuple(head, tail);
	}

	friend std::istream &operator>>(std::istream &is, Instruction &i)
	{
		// default initialize the most probable moved from object
		i = std::move(Instruction{});
		is >> i.dir;
		is >> i.amount;
		return is;
	}
};

std::set<std::array<int,2>> partA(std::vector<Instruction> instructions, int knots)
{
	std::array<int,2> head{0,0};
	std::vector<std::array<int,2>> tail(knots, {0,0});
	std::set<std::array<int,2>> visited{tail.back()};

	for(const auto& instr : instructions)
	{
		std::tie(head,tail) = instr.exec(head,tail, visited);

		// print positions only for debuggin
		// std::cout << "Head at: " << head[0] << "|" << head[1] << std::endl;
		// for(const auto& t : tail)
		// {
		// 	std::cout << "x at: " << t[0] << "|" << t[1] << " ";
		// }
		// std::cout << "\n" << std::endl;
	}

	return visited;
}

int main(int argc, char *argv[])
{
	// std::ifstream ifs{"../problems/day09.dat.testing"};
	// std::ifstream ifs{"../problems/day09.dat.testing2"};
	std::ifstream ifs{"../problems/day09.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Instruction> instructions = aocutils::vectorize_ifs<Instruction>(ifs);

	auto part1 = partA(instructions, 1).size();
	auto part2 = partA(instructions, 9).size();

	std::cout << "Day 09:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
