#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

#include "aocutils.h"

long solve_part1(std::vector<long> &v_lon, long key, int rounds)
{
	std::list<long> lon{v_lon.begin(), v_lon.end()};

	std::vector<std::list<long>::iterator> order{};
	order.reserve(lon.size());

	for(auto i = lon.begin(); i != lon.end(); i++)
	{
		(*i) *= key;
		order.push_back(i);
	}

	// for(const auto& i : order) {
	// 	std::cout << i->num << " ";
	// }
	// std::cout << std::endl;

	for(int i{0}; i < rounds; i++)
	{
		for(auto &i : order)
		{
			long adv{*(i) % static_cast<long>(lon.size() - 1)};

			if(adv == 0) { continue; }

			adv = adv < 0 ? adv : adv + 1;

			lon.splice(aocutils::advance_circle(lon, i, adv), lon, i, std::next(i, 1));
		}
	}

	auto zero =
		std::find_if(lon.begin(), lon.end(), [](auto &i) { return i == 0; });
	auto one   = aocutils::advance_circle(lon, zero, 1000 % lon.size());
	auto two   = aocutils::advance_circle(lon, zero, 2000 % lon.size());
	auto three = aocutils::advance_circle(lon, zero, 3000 % lon.size());

	// std::cout
	// 	<< one->get().num << " | "
	// 	<< two->get().num << " | "
	// 	<< three->get().num << std::endl
	// ;

	return (*one) + (*two) + (*three);
}

int main()
{
	// std::ifstream ifs{"../problems/day20.dat.testing3"};
	std::ifstream ifs{"../problems/day20.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<long> lon = aocutils::vectorize_ifs<long>(ifs);

	auto part1 = solve_part1(lon, 1, 1);
	auto part2 = solve_part1(lon, 811589153, 10);

	std::cout << "Day 20:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
