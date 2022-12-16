#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "aocutils.h"

int manhatten(std::pair<int,int> a, std::pair<int,int> b)
{
	return abs(a.first - b.first) + abs(a.second - b.second);
}

struct LogEntry
{
	std::pair<int,int> sensor;
	std::pair<int,int> beacon;
	int r_man;

	friend std::istream &operator>>(std::istream &is, LogEntry &i)
	{
		// default initialize the most probable moved from object
		i = std::move(LogEntry{});


		is.ignore(static_cast<std::streamsize>(std::string{"Sensor at x="}.length()));
		if(is.eof())
		{
			is.setstate(is.badbit); // avoid this element being inserted
			return is;
		}
		is >> i.sensor.first;
		if(is.eof()) return is;
		is.ignore(static_cast<std::streamsize>(std::string{", y="}.length()));
		is >> i.sensor.second;

		is.ignore(static_cast<std::streamsize>(std::string{": closest beacon is at x="}.length()));
		is >> i.beacon.first;
		is.ignore(static_cast<std::streamsize>(std::string{", y="}.length()));
		is >> i.beacon.second;

		i.r_man = manhatten(i.beacon, i.sensor);

		is.get(); // skip newline character
		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, const LogEntry &i)
	{
		os << "Sensor at x=" << i.sensor.first << ", y=" << i.sensor.second << ": closest beacon is at x=" << i.beacon.first << ", y=" << i.beacon.second << " has r_man=\n" << i.r_man;
		return os;
	}
};

int partA(std::vector<LogEntry> logs, int y)
{
	std::map<int,bool> line{}; // y-Coordinate will be similar anyhow
	for(const LogEntry& log : logs)
	{
		if(log.sensor.second == y) line[log.sensor.first] = true;
		if(log.beacon.second == y) line[log.beacon.first] = true;
		// std::cout << log << std::endl;
		int manhatten_ref{manhatten(log.beacon, log.sensor)};
		// std::cout << manhatten_ref << std::endl;
		for(int dir : {-1,1})
		{
			for(int x{log.sensor.first}; manhatten(log.sensor, {x,y}) <= manhatten_ref; x+=dir)
			{
				if(!line.contains(x))
					line[x] = false;
			}
		}
	}
	for(const auto& [x,y] : line)
		std::cout << x << " -> " << y << std::endl;

	std::cout << std::count_if(line.begin(), line.end(), [](const auto& x){return x.second == false;}) << std::endl;
	std::cout << std::count_if(line.begin(), line.end(), [](const auto& x){return x.second == true;}) << std::endl;
	return static_cast<int>(std::count_if(line.begin(), line.end(), [](const auto& x){return x.second == false;}));
}

// // TODO consider https://stackoverflow.com/a/20800862
// int partB(std::vector<LogEntry> logs, int max)
// {
// 	std::set<std::reference_wrapper<logs>> ordered_log{};
// 	for(const auto& log : logs)
// 	{
// 		// calculate highest point -> add to ordered_log
// 	}
// 	for(int y{0}; y <= max; y++)
// 	{
// 		// hold structure with active rectangles (add if y == ordered_log->highest point, remove is y == ordered_log->lowest_point)
// 		for(int x{0}; x <= max; x++)
// 		{
// 			// check if covered
// 			// TODO can this improved by skipping x-values since many consecutive one will be covered
// 		}
// 	}
// }

void dbg(std::vector<LogEntry> logs, int x, int y)
{
	for(const auto& log : logs)
	{
		std::cout << log << "\nx=" << x << "y=" << y << " has\n" << manhatten(log.sensor, {x,y}) << "\n" << std::endl;
	}
}

long partB(std::vector<LogEntry> logs, int max)
{
	for(const auto& log : logs)
	{
		for(const auto& dir : std::array<std::pair<int,int>,4>{std::make_pair(1,-1), std::make_pair(-1,-1), std::make_pair(-1,1), std::make_pair(1,1)})
		{
			// go from upper/lower corner around the edge
			for(int x{log.sensor.first}, y{log.sensor.second+(log.r_man+1)*(-dir.second)}; y != log.sensor.second; x+=dir.first, y+=dir.second)
			{
				bool covered = false;
				if(x < 0 || x > max || y < 0 || y > max) continue;
				for(const auto& log2 : logs)
				{
					if(manhatten(log2.sensor, {x,y}) <= log2.r_man) covered=true;

				}
				if(!covered)
				{
					// dbg(logs, x,y);
					return x*4000000l + y;
				}
			}
		}
	}
	return -1;
}

int main()
{
	// std::ifstream ifs{"../problems/day15.dat.testing"};
	std::ifstream ifs{"../problems/day15.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<LogEntry> logs = aocutils::vectorize_ifs<LogEntry>(ifs);


	// auto part1 = partA(logs, 2000000);
	auto part2 = partB(logs, 4000000);
	// auto part2 = partB(logs, 20);

	std::cout << "Day 15:" << '\n'
			  // << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
