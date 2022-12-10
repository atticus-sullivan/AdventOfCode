#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <vector>

#include "aocutils.h"

struct FSele
{
	std::string name;
	FSele(std::string name) : name{name} {}
	virtual ~FSele() = default;

	virtual int  get_size() const                = 0;
	virtual bool is_dir() const                  = 0;
	virtual int  sum_smaller_dir_than(int) const = 0;
	virtual int  bigger_than(int, int) const     = 0;
};
struct FSfile : FSele
{
	int size;
	FSfile(std::string name, int size) : FSele(name), size{size} {}

	int  get_size() const override { return size; }
	bool is_dir() const override { return false; }
	int  sum_smaller_dir_than(int) const override
	{
		throw std::runtime_error("Not implemented");
	};
	int bigger_than(int, int) const override
	{
		throw std::runtime_error("Not implemented");
	};
};
struct FSdir : FSele
{
	std::map<std::string, std::unique_ptr<FSele>> children;
	FSdir(std::string name) : FSele(name), children{} {}

	bool is_dir() const override { return true; }

	int get_size() const override
	{
		int size{0};
		for(const auto &[k, v] : children) { size += v->get_size(); }
		return size;
	}

	int sum_smaller_dir_than(int size) const override
	{
		int sum{0};
		for(const auto &[k, v] : children)
		{
			if(v->is_dir()) { sum += v.get()->sum_smaller_dir_than(size); }
		}
		if(get_size() < size)
		{
			// std::cout << name << " " << get_size() << std::endl;
			sum += get_size();
		}
		return sum;
	}

	int bigger_than(int size, int min_bigger_size) const override
	{
		for(const auto &[k, v] : children)
		{
			if(v->is_dir())
			{
				min_bigger_size = v.get()->bigger_than(size, min_bigger_size);
			}
		}
		if(get_size() > size)
		{ // std::cout << name << " " << get_size() << " vs " << min_bigger_size
		  // << std::endl;
			min_bigger_size = std::min(min_bigger_size, get_size());
			// std::cout << min_bigger_size << std::endl;
		}
		return min_bigger_size;
	}
};

FSdir parse(std::ifstream ifs)
{
	// use a reference_wrapper as references are not allowed in stl containers
	std::vector<std::reference_wrapper<FSdir>> path{};
	std::string                                l{};
	std::string                                name{};

	std::getline(ifs, l); // skip $ cd /
	FSdir root{"/"};
	path.push_back(std::ref(root));

	ifs >> l; // $
	while(ifs.good())
	{
		ifs >> l;
		// std::cout << "command: " << l << std::endl;
		if(l == "cd")
		{
			ifs >> l;
			if(l == "..") path.pop_back();
			else
				// exception if node is not already known from ls
				path.push_back(std::ref(dynamic_cast<FSdir &>(
					*path.back().get().children.at(l).get())));
			ifs >> l; // $
		}
		else if(l == "ls")
		{
			ifs >> l;
			while(l != "$" && ifs.good())
			{
				ifs >> name;
				// std::cout << l << " " << name << std::endl;
				if(l == "dir")
					path.back().get().children.insert(
						{name, std::make_unique<FSdir>(FSdir{name})});
				else
					path.back().get().children.insert(
						{name,
						 std::make_unique<FSfile>(FSfile{name, std::stoi(l)})});
				ifs >> l;
			}
		}
		else
		{
			ifs >> l;
			std::cout << "next would have been: " << l << std::endl;
			throw std::runtime_error("Invalid command");
		}
	}
	return root;
}

int partB(const FSdir &root)
{
	int need_to_free = 30000000 - (70000000 - root.get_size());
	// std::cout << "needs: " << need_to_free << std::endl;
	return root.bigger_than(need_to_free, std::numeric_limits<int>::max());
}

int main()
{
	// std::ifstream ifs{"../problems/day07.dat.testing"};
	std::ifstream ifs{"../problems/day07.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	FSdir root = std::move(parse(std::move(ifs)));

	// std::cout << root.get_size() << std::endl;

	int part1 = root.sum_smaller_dir_than(100000);
	int part2 = partB(root);

	std::cout << "Day 04:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
