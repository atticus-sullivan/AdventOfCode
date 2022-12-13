#include <cassert>
#include <fstream>
#include <iostream>
#include <variant>
#include <vector>
#include <limits>
#include <stack>
#include <set>

#include "aocutils.h"

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

static bool dbg = false;

struct List
{
	std::vector<std::variant<int,List>> l;

	List(List x, bool) : l{x} {}
	List(const int& x) : l{x} {
		if(dbg)
			std::cout << "constructed with " << x << std::endl;
	};
	List() = default;

	friend int cmp(const List& a, const List& b)
	{
		for(int i{0}; i < std::min(a.l.size(), b.l.size()); i++)
		{
			if(dbg)
			{
				std::cout << "compare ";
				if(a.l.at(i).index() == 0) std::cout << std::get<0>(a.l.at(i));
				else std::cout << std::get<1>(a.l.at(i));
				std::cout << " with ";
				if(b.l.at(i).index() == 0) std::cout << std::get<0>(b.l.at(i));
				else std::cout << std::get<1>(b.l.at(i));
				std::cout << std::endl;
			}

			int res{0};
			if(a.l.at(i).index() == 0)
			{
				if(b.l.at(i).index() == 0)
				{
					res = -aocutils::sgn(std::get<0>(a.l.at(i)) - std::get<0>(b.l.at(i)));
				}
				else { res = cmp(std::get<0>(a.l.at(i)), std::get<1>(b.l.at(i))); }
			} else {
				if(b.l.at(i).index() == 0) { res = cmp(std::get<1>(a.l.at(i)), std::get<0>(b.l.at(i))); }
				else { res = cmp(std::get<1>(a.l.at(i)), std::get<1>(b.l.at(i))); }
			}
			if(dbg)
				std::cout << res << std::endl;
			if(res == 1) return 1;
			if(res == -1) return -1;
		}
		if(dbg)
			std::cout << "decision based on size -(" << a.l.size() << " - " << b.l.size() << ")" << std::endl;
		return -aocutils::sgn(static_cast<int>(a.l.size()) - static_cast<int>(b.l.size()));
	}

	friend std::istream &operator>>(std::istream &is, List &i)
	{
		char c{};

		while(true)
		{
			do {
				is.get(c);
			} while(c == ',');
			// std::cout << "c " << c << std::endl;
			if(c == '[')
			{
				// std::cout << "go deeper" << std::endl;
				List l{}; is >> l;
				i.l.emplace_back(l);
			} else if (c == ']')
			{
				// std::cout << "go back" << std::endl;
				return is;
			} else if (c == '\n' || is.eof())
			{
				return is;
				// std::cout << "terminate" << std::endl;
			} else {
				is.putback(c);
				int j{};
				is >> j;
				i.l.emplace_back(j);
				// std::cout << "j " << j << std::endl;
			}
		}

		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, const List &i)
	{
		os << "[";
		for(const auto& ele: i.l)
		{
			if(ele.index() == 0)
				os << std::get<0>(ele) << " ";
			else
				os << std::get<1>(ele) << " ";
		}
		os << "]";
		return os;
	}
};
struct Pair
{
	std::pair<List,List> p;

	bool right_order()
	{
		return cmp(p.first, p.second) == 1;
	}

	friend std::istream &operator>>(std::istream &is, Pair &i)
	{
		// default initialize the most probable moved from object
		i = std::move(Pair{});

		// std::cout << "first" << std::endl;
		is >> i.p.first;
		if(is.eof()) return is;
		// std::cout << "snd" << std::endl;
		is >> i.p.second;
		if(is.eof()) return is;

		is.ignore(max_size, '\n');

		// std::cout << i.p.first << std::endl;
		// std::cout << i.p.second << std::endl;
		// std::cout << "\n" << std::endl;

		return is;
	}
	// friend std::ostream &operator<<(std::ostream &os, Pair &i)
	// {
	// 	throw std::runtime_error("Printing not implemented");
	// 	return os;
	// }
};

int main()
{
	// std::ifstream ifs{"../problems/day13.dat.testing"};
	std::ifstream ifs{"../problems/day13.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Pair> pairs = aocutils::vectorize_ifs<Pair>(ifs);

	auto tmp1 = pairs | std::views::transform([](const auto& a){return a.p.first;});
	auto tmp2 = pairs | std::views::transform([](const auto& a){return a.p.second;});

	List d1(List(List(2), true), true);
	List d2(List(List(6), true), true);

	auto c = [](const auto& a, const auto& b){return cmp(a, b) == 1;};

	std::set<List, decltype(c)> sorted{tmp1.begin(), tmp1.end()};
	sorted.insert(tmp2.begin(), tmp2.end());

	sorted.insert(d1);
	sorted.insert(d2);

	int d1_idx = std::distance(sorted.begin(), sorted.find(d1))+1;
	int d2_idx = std::distance(sorted.begin(), sorted.find(d2))+1;

	// for(const auto& s: sorted)
	// 	std::cout << s << std::endl;

	int part1{0};
	for(int i{0}; i < pairs.size(); i++)
	{
		if(pairs.at(i).right_order())
		{
			// std::cout << "add " << i+1 << std::endl;
			part1+=i+1;
		}
		// std::cout << "\n" << std::endl;
	}

	auto part2 = d1_idx * d2_idx;

	std::cout << "Day 13:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
