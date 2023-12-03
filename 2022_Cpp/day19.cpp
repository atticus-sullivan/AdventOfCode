#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "aocutils.h"

struct Blueprint
{
	uint                               id;
	std::array<std::array<uint, 3>, 4> costs;
	std::array<uint, 3>                maxCosts;

	friend std::istream &operator>>(std::istream &is, Blueprint &i)
	{
		// default initialize the most probable moved from object
		i = std::move(Blueprint{});

		is.ignore(10);
		is >> i.id;

		is.ignore(23);
		is >> i.costs[0][0];

		is.ignore(28);
		is >> i.costs[1][0];

		is.ignore(32);
		is >> i.costs[2][0];
		is.ignore(9);
		is >> i.costs[2][1];

		is.ignore(30);
		is >> i.costs[3][0];
		is.ignore(9);
		is >> i.costs[3][2];

		is.ignore(10);

		auto tmp0 = i.costs | std::views::transform([](std::array<uint, 3> &i)
		{ return i[0]; });
		i.maxCosts[0] = std::ranges::max(tmp0);
		auto tmp1 = i.costs | std::views::transform([](std::array<uint, 3> &i)
		{ return i[1]; });
		i.maxCosts[1] = std::ranges::max(tmp1);
		auto tmp2 = i.costs | std::views::transform([](std::array<uint, 3> &i)
		{ return i[2]; });
		i.maxCosts[2] = std::ranges::max(tmp2);

		return is;
	}

	friend std::ostream &operator<<(std::ostream &os, const Blueprint &i)
	{
		os << "{id:" << i.id << " ore:" << i.costs[0][0] << "," << i.costs[0][1]
			<< "," << i.costs[0][2] << " clay:" << i.costs[1][0] << ","
			<< i.costs[1][1] << "," << i.costs[1][2]
			<< " obsidian:" << i.costs[2][0] << "," << i.costs[2][1] << ","
			<< i.costs[2][2] << " geode:" << i.costs[3][0] << ","
			<< i.costs[3][1] << "," << i.costs[3][2] << "}";
		return os;
	}
};

struct State
{
	std::array<uint, 4> per_min;
	std::array<uint, 4> stock;
	uint                minutes_left;
};

uint max_geodes(const Blueprint &blueprint, const State &&state_orig, uint max)
{
	uint max_g{state_orig.stock[3] +
		state_orig.per_min[3] * state_orig.minutes_left};

	if(((state_orig.minutes_left * (state_orig.minutes_left + 1) / 2) +
		state_orig.stock[3] + state_orig.minutes_left * state_orig.per_min[3]) <
	max)
	{
		// this idea is from
		// https://todd.ginsberg.com/post/advent-of-code/2022/day19/ pretend
		// we're building one robot for geode cracking in each minute till the
		// end (best case), can we produce more geodes than another series of
		// states could? if not, abort since there is no way this state can lead
		// to a more cracked gedes than the current maximum
		return 0;
	}

	for(size_t j{0}; j < blueprint.costs.size(); j++)
	{
		if(j < blueprint.costs.size() - 1 &&
			state_orig.per_min[j] >= blueprint.maxCosts[j])
		{
			// this idea is from
			// https://todd.ginsberg.com/post/advent-of-code/2022/day19/
			// exception: blueprint is for geode
			// if we already produce more than we can use in building, skip this
			continue;
		}
		// check how long to save before building that robot
		int time{-1};
		for(size_t i{0}; i < blueprint.costs[j].size(); i++)
		{
			if(state_orig.per_min[i] != 0)
			{
				if(blueprint.costs[j][i] <= state_orig.stock[i])
				{
					time = std::max(time, 0);
				}
				else
				{
					time = std::max(
						time, static_cast<int>(std::ceil(
							static_cast<float>(blueprint.costs[j][i] -
							state_orig.stock[i]) /
							static_cast<float>(state_orig.per_min[i]))));
				}
			}
			else if(blueprint.costs[j][i] != 0 && state_orig.per_min[i] == 0)
			{
				// blueprint not possible
				time = static_cast<int>(state_orig.minutes_left + 1);
				break;
			}
		}
		// check if possible to build that robot
		if(time >= 0 && state_orig.minutes_left >= static_cast<uint>(time + 1))
		{
			// can construct ore robot
			State state{state_orig};
			// "simulate time"
			for(size_t i{0}; i < state.stock.size(); i++)
			{
				state.stock[i] +=
					state.per_min[i] * static_cast<uint>(time + 1);
			}
			// construct ore robot
			for(size_t i{0}; i < blueprint.costs[j].size(); i++)
			{
				state.stock[i] -= blueprint.costs[j][i];
			}
			state.per_min[j] += 1;
			state.minutes_left -= static_cast<uint>(time + 1);
			if(state.minutes_left > 0)
			{
				// recurse
				max_g = std::max(
					max_g, max_geodes(blueprint, std::move(state), max_g));
			}
			else
			{
				max_g =
					std::max(max_g, state.stock[3] +
			  state.per_min[3] * state.minutes_left);
			}
		}
	}

	return max_g;
}

int main()
{
	// std::ifstream ifs{"../problems/day19.dat.testing"};
	std::ifstream ifs{"../problems/day19.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Blueprint> blueprints = aocutils::vectorize_ifs<Blueprint>(ifs);

	auto part1{0};
	for(const auto &b : blueprints)
	{
		auto x{
			max_geodes(b, State{{1, 0, 0, 0}, {}, 24},
			  0)
		};
		// std::cout << b.id << " | " << x << std::endl;
		part1 += b.id * x;
	}

	auto part2{1};
	for(auto b = blueprints.begin(); b != blueprints.begin() + 3; b++)
	{
		auto x{
			max_geodes(*b, State{{1, 0, 0, 0}, {}, 32},
			  0)
		};
		// std::cout << b->id << " | " << x << std::endl;
		part2 *= x;
	}

	std::cout << "Day 19:" << '\n'
		<< "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << part2 << std::endl;
}
