#include <cassert>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>

#include "aocutils.h"

enum class Direction
{
	RIGHT, UP, DOWN, LEFT,
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

	template <std::size_t n_knots>
	void exec(std::array<std::array<int,2>, n_knots> &knots, std::set<std::array<int,2>>& track) const
	{
		using enum Direction;
		for(int i{0}; i < amount; i++)
		{
			// move head
			switch(dir)
			{
				case RIGHT:
					knots.front()[0]++;
					break;
				case LEFT:
					knots.front()[0]--;
					break;
				case UP:
					knots.front()[1]--;
					break;
				case DOWN:
					knots.front()[1]++;
					break;
				default: throw std::runtime_error("Invalid direction");
			}
			// move all knots (except the first/head one)
			for(auto last = knots.begin(), k = knots.begin()+1; k != knots.end(); last++, k++)
			{
				if((*k)[0] == (*last)[0])
				{
					// same column
					if(abs((*last)[1] - (*k)[1]) > 1) (*k)[1] += aocutils::sgn((*last)[1] - (*k)[1]);
				} else if((*k)[1] == (*last)[1]) {
					// same row
					if(abs((*last)[0] - (*k)[0]) > 1) (*k)[0] += aocutils::sgn((*last)[0] - (*k)[0]);
				} else if((abs((*last)[0] - (*k)[0]) >= 1 && abs((*last)[1] - (*k)[1]) > 1) || (abs((*last)[0] - (*k)[0]) > 1 && abs((*last)[1] - (*k)[1]) >= 1)){
					// diagonal
					(*k)[0] += aocutils::sgn((*last)[0] - (*k)[0]);
					(*k)[1] += aocutils::sgn((*last)[1] - (*k)[1]);
				}
			}
			track.emplace(knots.back());
		}
	}

	friend std::ostream &operator<<(std::ostream &os, const Instruction &i)
	{
		os << i.dir << " " << i.amount;
		return os;
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

template <std::size_t n_knots>
std::set<std::array<int,2>> partAll(std::vector<Instruction> instructions)
{
	std::array<std::array<int,2>, n_knots> knots{};
	for(int i{0}; i < n_knots; i++)
		knots[i] = {0,0};
	std::set<std::array<int,2>> visited{knots.back()};

	for(const auto& instr : instructions)
	{
		instr.exec<n_knots>(knots, visited);

		// // print positions only for debugging
		// std::cout << instr << std::endl;
		// for(const auto& k : knots)
		// {
		// 	std::cout << "x at: " << k[0] << "|" << k[1] << " ";
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

	auto part1 = partAll<2>(instructions).size();
	auto part2 = partAll<10>(instructions).size();

	std::cout << "Day 09:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
