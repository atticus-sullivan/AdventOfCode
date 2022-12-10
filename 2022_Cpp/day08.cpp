#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <vector>

#include "aocutils.h"

struct Field
{
	std::vector<int>  vec;
	std::vector<bool> vis;
	std::vector<int>  scores;
	int               dim;

	void set_scenic_score(int x, int y)
	{
		int current{vec.at(static_cast<unsigned>(y * dim + x))};
		int score{1}; // neutral element of multiplication
		// go in each direction once (not diagonal)
		for(const auto &[i, j] :
			{std::make_tuple(1, 0), std::make_tuple(0, 1),
			 std::make_tuple(-1, 0), std::make_tuple(0, -1)})
		{
			int cnt{0}; // neutral element of addition
			// start checking at coordinate (k,l)
			int k{x + i};
			int l{y + j};
			while(0 <= k && k < dim && 0 <= l && l < dim)
			{
				cnt++;
				// stop if tree is too high => everything beind is not visible
				// anymore
				if(vec.at(static_cast<unsigned>(l * dim + k)) >= current) break;
				k += i;
				l += j;
			}
			score *= cnt;
			// 0*x will always stay 0
			if(score == 0) break;
		}
		scores.at(static_cast<unsigned>(y * dim + x)) = score;
	}

	friend std::istream &operator>>(std::istream &is, Field &i)
	{
		std::string l{};
		std::getline(is, l);

		// init vector-sizes of i (avoid reallocation)
		i.dim    = static_cast<int>(l.length());
		i.vec    = std::vector<int>(static_cast<unsigned>(i.dim * i.dim));
		i.vis    = std::vector<bool>(static_cast<unsigned>(i.dim * i.dim));
		i.scores = std::vector<int>(static_cast<unsigned>(i.dim * i.dim));
		int idx{0};

		// populate with already read line (needed to determine the dimension of
		// the input)
		for(const char c : l)
		{
			i.vis.at(static_cast<unsigned>(idx))    = false;
			i.scores.at(static_cast<unsigned>(idx)) = 0;
			i.vec.at(static_cast<unsigned>(idx))    = c - '0';
			idx++;
		}

		// read rest of the input
		char c;
		is >> c;
		for(; is.good(); is >> c)
		{
			i.vis.at(static_cast<unsigned>(idx))    = false;
			i.scores.at(static_cast<unsigned>(idx)) = 0;
			i.vec.at(static_cast<unsigned>(idx))    = c - '0';
			idx++;
		}

		// set scenic score for each position
		for(int y{0}; y < i.dim; y++)
		{
			for(int x{0}; x < i.dim; x++) { i.set_scenic_score(x, y); }
		}

		// check what is visible from the outside looking from left and right
		for(auto [x_, k] :
			{std::make_tuple(0, 1), std::make_tuple(i.dim - 1, -1)})
		{
			int y{0};
			while(0 <= y && y < i.dim)
			{
				int x{x_};
				int max{-1};
				while(0 <= x && x < i.dim)
				{
					if(max < i.vec.at(static_cast<unsigned>(y * i.dim + x)))
					{
						i.vis.at(static_cast<unsigned>(y * i.dim + x)) = true;
						max = i.vec.at(static_cast<unsigned>(y * i.dim + x));
					}
					x += k;
				}
				y += 1;
			}
		}
		// check what is visible from the outside looking from top and bottom
		for(auto [y_, l] :
			{std::make_tuple(0, 1), std::make_tuple(i.dim - 1, -1)})
		{
			int x{0};
			while(0 <= x && x < i.dim)
			{
				int y{y_};
				int max{-1};
				while(0 <= y && y < i.dim)
				{
					if(max < i.vec.at(static_cast<unsigned>(y * i.dim + x)))
					{
						i.vis.at(static_cast<unsigned>(y * i.dim + x)) = true;
						max = i.vec.at(static_cast<unsigned>(y * i.dim + x));
					}
					y += l;
				}
				x += 1;
			}
		}
		return is;
	}
};

int main()
{
	// std::ifstream ifs{"../problems/day08.dat.testing"};
	std::ifstream ifs{"../problems/day08.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	Field f{};
	ifs >> f;

	auto part1_ = f.vis | std::views::transform(
							  [](const bool a)
							  {
								  if(a) return 1;
								  else return 0;
							  });
	auto part1 = std::accumulate(part1_.begin(), part1_.end(), 0);
	auto part2 = *std::max_element(f.scores.begin(), f.scores.end());

	std::cout << "Day 08:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
