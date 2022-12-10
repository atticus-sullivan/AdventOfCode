#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "aocutils.h"

struct Message
{
	std::string msg;

	int search_start(unsigned int count)
	{
		auto left{msg.begin()};
		auto right{msg.begin()};
		// storee how often which element occurs
		std::map<char, int> eleMap{};

		unsigned int j{0}; // how many characters are being read so far
		// initialize with the first count characters
		for(; j < count; j++, right++) { eleMap[*right]++; }
		// shift window one char to the right each time
		for(; right != msg.end(); right++, left++, j++)
		{
			// count distict elements contained in Map
			if(eleMap.size() == count) return static_cast<int>(j);
			// left char occurs one time less in range, check if it is to be
			// removed
			if(--eleMap[*left] == 0) eleMap.erase(*left);
			// right character occurs one time more in range
			eleMap[*right]++;
		}
		return -1;
	}

	friend std::istream &operator>>(std::istream &is, Message &i)
	{
		// default initialize the most probable moved from object
		i = std::move(Message{});

		std::getline(is, i.msg);
		return is;
	}
};

int main()
{
	// std::ifstream ifs{"../problems/day06.dat.testing"};
	std::ifstream ifs{"../problems/day06.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	Message m{};
	ifs >> m;

	int part1 = m.search_start(4);
	int part2 = m.search_start(14);

	std::cout << "Day 04:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
