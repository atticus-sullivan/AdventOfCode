#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include <deque>
#include <limits>
#include <sstream>
#include <numeric>
#include <ranges>

#include "aocutils.h"

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

bool is_number(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

std::function<long(long)> func_factory(std::string op1, std::string op, std::string op2)
{
	std::function<long(long,long)> opl{};
	char o = op.at(0);
	switch(o)
	{
		case '*':
			opl = [](long op1, long op2){return op1*op2;};
			break;
		case '+':
			opl = [](long op1, long op2){return op1+op2;};
			break;
		default:
			throw std::runtime_error{"Invalid operation"};
	}

	std::function<long(long)> ret{};
	if(op1 == "old" && op2 == "old")
	{
		ret = [opl](long old) {return opl(old,old);};
	}
	else if(op1 == "old")
	{
		assert(is_number(op2));
		long op2i{std::stol(op2)};
		ret = [opl,op2i](long old) {return opl(old,op2i);};
	} else
	{
		assert(is_number(op1));
		assert(is_number(op2));
		long op1i{std::stol(op2)};
		long op2i{std::stol(op2)};
		ret = [opl,op1i,op2i](long _) {return opl(op1i,op2i);};
	}
	return ret;
}

struct Monkey
{
	std::deque<long> items;
	std::function<long(long)> op;
	int div_able;
	unsigned int throw_true;
	unsigned int throw_false;
	unsigned long inspected;

	void exec(std::vector<Monkey>& monkeys, std::function<long(long)> red)
	{
		long item{};
		while(!items.empty()){

			item = items.front();
			items.pop_front();

			// inspect
			inspected++;
			item = op(item);
			// reduce
			item = red(item);
			// tests your worry level
			if(item % div_able == 0)
			{
				monkeys.at(throw_true).items.push_back(item);
			} else
			{
				monkeys.at(throw_false).items.push_back(item);
			}
		}
	}

	friend std::istream &operator>>(std::istream &is, Monkey &i)
	{
		// default initialize the most probable moved from object
		i = std::move(Monkey{});

		std::string l{};
		is.ignore(max_size, '\n'); // Monkey Nr.

		// items
		is.ignore(max_size, ':');
		if(!is.good()) return is;
		std::getline(is, l);
		std::istringstream ls{l};
		int j{};
		while(ls.good())
		{
			ls >> j;
			ls.ignore(1);
			// std::cout << j << std::endl;
			i.items.push_back(j);
		}

		// operation
		is.ignore(max_size, ':');
		if(!is.good()) return is;
		is >> l;
		assert(l == "new");
		is >> l;
		assert(l == "=");
		std::string op1{};
		is >> op1;
		std::string op{};
		is >> op;
		std::string op2{};
		is >> op2;
		i.op = func_factory(op1, op, op2);

		// div able
		is.ignore(max_size, ':');
		is >> l;
		assert(l == "divisible");
		is >> l;
		assert(l == "by");
		is >> i.div_able;

		// true
		is >> l;
		assert(l == "If");
		is >> l;
		assert(l == "true:");
		is >> l;
		assert(l == "throw");
		is >> l;
		assert(l == "to");
		is >> l;
		assert(l == "monkey");
		is >> i.throw_true;

		// false
		is >> l;
		assert(l == "If");
		is >> l;
		assert(l == "false:");
		is >> l;
		assert(l == "throw");
		is >> l;
		assert(l == "to");
		is >> l;
		assert(l == "monkey");
		is >> i.throw_false;

		std::getline(is, l);
		std::getline(is, l); // TODO

		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, const Monkey &i)
	{
		for(const auto& ele : i.items)
		{
			os << ele << " ";
		}
		return os;
	}
};

long lA(long a){
	return a/3;
}
unsigned long partA(std::vector<Monkey> monkeys)
{
	// std::cout << "\n";
	// std::cout << monkeys.size() << std::endl;
	// for(const auto& m : monkeys){
	// 	std::cout << m;
	// }
	// std::cout << "\n";


	for(int i{0}; i < 20; i++){
		for(auto& m : monkeys){
			m.exec(monkeys, lA);
		}
		// std::cout << "\n";
		// std::cout << monkeys.size() << std::endl;
		// for(const auto& m : monkeys){
		// 	std::cout << m << "(" << m.inspected << ")" << std::endl;
		// }
	}

	std::array<Monkey, 2> top{};
	std::partial_sort_copy(
			monkeys.begin(), monkeys.end(),
			top.begin(), top.end(),
			[](const Monkey& a, const Monkey& b){return a.inspected > b.inspected;}
			);


	// std::cout << top[0].inspected << top[1].inspected << std::endl;
	auto part1 = top[0].inspected * top[1].inspected;
	return part1;
}

unsigned long partB(std::vector<Monkey> monkeys)
{
	// std::cout << "\n";
	// std::cout << monkeys.size() << std::endl;
	// for(const auto& m : monkeys){
	// 	std::cout << m;
	// }
	// std::cout << "\n";

	auto lcm = monkeys | std::views::transform([](const Monkey& a){return a.div_able;});
	const int ring = std::reduce(lcm.begin(), lcm.end(), 1, [](const int a, const int b){return std::lcm(a, b);});
	// std::cout << "Ring: " << ring << std::endl;

	for(int i{0}; i < 10000; i++){
		for(auto& m : monkeys){
			m.exec(monkeys, [ring](long a){return a%ring;});
		}
		// if(i+1 == 1 || i+1 == 20 || (i+1) % 1000 == 0)
		// {
		// 	std::cout << "\n";
		// 	std::cout << "Round: " << i+1 << std::endl;
		// 	for(const auto& m : monkeys){
		// 		std::cout << m << "(" << m.inspected << ")" << std::endl;
		// 	}
		// }
	}

	std::array<Monkey, 2> top{};
	std::partial_sort_copy(
			monkeys.begin(), monkeys.end(),
			top.begin(), top.end(),
			[](const Monkey& a, const Monkey& b){return a.inspected > b.inspected;}
			);


	// std::cout << top[0].inspected << " " << top[1].inspected << std::endl;
	auto part1 = top[0].inspected * top[1].inspected;
	return part1;
}

int main()
{
	// std::ifstream ifs{"../problems/day11.dat.testing"};
	std::ifstream ifs{"../problems/day11.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Monkey> monkeys = aocutils::vectorize_ifs<Monkey>(ifs);

	auto part1 = partA(monkeys);
	auto part2 = partB(monkeys);

	std::cout << "Day 11:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
