/*
Advent Of Code 2020
Copyright (C) 2024  Lukas Heindl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

#include "aocutils.h"

enum Operation
{
	op_add,
	op_sub,
	op_mul,
	op_div
};

bool is_number(const std::string &s)
{
	return !s.empty() && std::find_if(s.begin(), s.end(),
									  [](unsigned char c)
									  { return !std::isdigit(c); }) == s.end();
}

struct Monkey
{
	std::string name;
	long        num;
	bool        compound;
	std::string monk_a;
	std::string monk_b;
	Operation   op;

	friend std::istream &operator>>(std::istream &is, Monkey &i)
	{
		// default initialize the most probable moved from object
		i = std::move(Monkey{});

		is >> i.name;
		i.name.pop_back();
		std::string w{};
		is >> w;
		if(is_number(w))
		{
			i.compound = false;
			i.num      = std::stoi(w);
		}
		else
		{
			i.compound = true;
			i.monk_a   = std::move(w);

			char o{};
			is.ignore(1);
			is >> o;
			switch(o)
			{
				case '+':
					i.op = op_add;
					break;
				case '-':
					i.op = op_sub;
					break;
				case '*':
					i.op = op_mul;
					break;
				case '/':
					i.op = op_div;
					break;
				default:
					break; // throw std::runtime_error("Invalid operand read");
			}

			is >> i.monk_b;
		}

		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, const Monkey &i)
	{
		os << i.name << ": ";
		if(i.compound) { os << i.monk_a << " " << i.op << " " << i.monk_b; }
		else { os << i.num; }
		return os;
	}
};

long solve_a(std::map<std::string, Monkey> &monkeys, Monkey &root)
{
	if(!root.compound) { return root.num; }
	else
	{
		auto a{solve_a(monkeys, monkeys[root.monk_a])};
		auto b{solve_a(monkeys, monkeys[root.monk_b])};
		switch(root.op)
		{
			case op_add:
				root.compound = false;
				root.num      = a + b;
				return root.num;
			case op_sub:
				root.compound = false;
				root.num      = a - b;
				return root.num;
			case op_mul:
				root.compound = false;
				root.num      = a * b;
				return root.num;
			case op_div:
				root.compound = false;
				root.num      = a / b;
				return root.num;
			default:
				throw std::runtime_error("invalid op");
		}
	}
}

std::optional<long> solve_ba(std::map<std::string, Monkey> &monkeys,
							 Monkey                        &root)
{
	if(root.name == "humn") { return {}; }
	if(!root.compound) { return root.num; }
	else
	{
		auto a{solve_ba(monkeys, monkeys[root.monk_a])};
		auto b{solve_ba(monkeys, monkeys[root.monk_b])};
		if(!a || !b) return {};
		switch(root.op)
		{
			case op_add:
				root.compound = false;
				root.num      = a.value() + b.value();
				return root.num;
			case op_sub:
				root.compound = false;
				root.num      = a.value() - b.value();
				return root.num;
			case op_mul:
				root.compound = false;
				root.num      = a.value() * b.value();
				return root.num;
			case op_div:
				root.compound = false;
				root.num      = a.value() / b.value();
				return root.num;
			default:
				throw std::runtime_error("invalid op");
		}
	}
}

long solve_bb(std::map<std::string, Monkey> &monkeys, Monkey &root, long should)
{
	// std::cout << root.name << std::endl;
	if(root.name == "humn")
	{
		// std::cout << "here" << std::endl;
		return should;
	}

	if(!monkeys[root.monk_a].compound)
	{
		auto a = monkeys[root.monk_a].num;
		switch(root.op)
		{
			case op_add:
				should = should - a;
				break;
			case op_sub:
				should = a - should;
				break;
			case op_mul:
				should = should / a;
				break;
			case op_div:
				should = a / should;
				break;
		}
		return solve_bb(monkeys, monkeys[root.monk_b], should);
	}
	else if(!monkeys[root.monk_b].compound)
	{
		auto b = monkeys[root.monk_b].num;
		switch(root.op)
		{
			case op_add:
				should = should - b;
				break;
			case op_sub:
				should = should + b;
				break;
			case op_mul:
				should = should / b;
				break;
			case op_div:
				should = should * b;
				break;
		}
		return solve_bb(monkeys, monkeys[root.monk_a], should);
	}
	else { throw std::runtime_error("invalid scenario (2)"); }
}

long solve_b(std::map<std::string, Monkey> &monkeys, Monkey &root)
{
	if(!root.compound) { return root.num; }
	else
	{
		auto a{solve_ba(monkeys, monkeys[root.monk_a])};
		auto b{solve_ba(monkeys, monkeys[root.monk_b])};

		if(a) { return solve_bb(monkeys, monkeys[root.monk_b], a.value()); }
		else if(b)
		{
			return solve_bb(monkeys, monkeys[root.monk_a], b.value());
		}
		else { throw std::runtime_error("invalid scenario"); }
	}
}

int main()
{
	// std::ifstream ifs{"../problems/day21.dat.testing"};
	std::ifstream ifs{"../problems/day21.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Monkey> instructions = aocutils::vectorize_ifs<Monkey>(ifs);

	// for(const auto& i : instructions) {
	// 	std::cout << i << std::endl;
	// }

	Monkey                        root{};
	std::map<std::string, Monkey> monkeys{};
	for(const auto &i : instructions)
	{
		if(i.name == "root") root = i;
		monkeys.insert({i.name, i});
	}

	auto part1 = solve_a(monkeys, root);

	monkeys.clear();
	for(const auto &i : instructions)
	{
		if(i.name == "root") root = i;
		monkeys.insert({i.name, i});
	}
	monkeys["humn"].compound = true;

	auto part2 = solve_b(monkeys, root);

	std::cout << "Day 21:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
