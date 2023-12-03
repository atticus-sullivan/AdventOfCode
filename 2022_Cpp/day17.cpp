#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <tuple>
#include <vector>

#include "aocutils.h"

using field_t = std::vector<std::array<bool, 7>>;

struct Flow
{
	int dir;

	friend std::istream &operator>>(std::istream &is, Flow &i)
	{
		// default initialize the most probable moved from object
		i = Flow{};
		char dir{};
		is >> dir;
		switch(dir) {
			case '<':
				i.dir = -1;
				break;
			case '>':
				i.dir = +1;
				break;
			case '\0':
				break;
			default:
				throw std::runtime_error("Invalid char");
		}
		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, Flow &i)
	{
		os << i.dir;
		return os;
	}
};

struct FallingRock
{
	FallingRock(std::vector<std::pair<int, int>> coords_,
				std::pair<int, int>              anchor_)
		: coords{std::move(coords_)}, anchor{std::move(anchor_)}
	{
		// as make_solid iterates over the coordinates and adds rows in the
		// field as needed, the coordinates must be sorted with respect to the y
		// component
		std::sort(coords.begin(), coords.end(), [](std::pair<int,int> a, std::pair<int,int> b){return a.second < b.second;});
		width = std::ranges::max_element(coords.begin(), coords.end(), {}, [](std::pair<int,int>& p){return p.first;})->first;
		height = std::ranges::max_element(coords.begin(), coords.end(), {}, [](std::pair<int,int>& p){return p.second;})->second;
	}
	FallingRock(std::vector<std::pair<int, int>> coords_)
		: FallingRock(coords_, {})
	{}

	void set_anchor(const field_t& field)
	{
		anchor.first = 2;
		anchor.second = static_cast<int>(field.size())+3;
	}

	bool move_right_left(const field_t& field, int dir)
	{
		if(anchor.first + width + dir >= 7 || anchor.first + dir < 0)
		{
			return false;
		}

		for(const auto& c : coords){
			int x{anchor.first + dir + c.first};
			int y{anchor.second + c.second};
			if(y < 0)
			{
				return false;
			}
			if(y >= static_cast<int>(field.size()))
				continue;
			// check if field is already occupied
			if(field.at(y).at(x)) return false;
		}

		anchor.first += dir;
		return true;
	}

	bool move_down(const field_t& field)
	{
		for(const auto& c : coords){
			int x{anchor.first + c.first};
			int y{anchor.second + c.second -1};
			if(y < 0)
			{
				return false;
			}
			if(y >= static_cast<int>(field.size()))
				continue;
			// check if field is already occupied
			if(field.at(y).at(x))
			{
				return false;
			}
		}
		anchor.second--;
		return true;
	}

	void make_solid(field_t& field)
	{
		for(const auto& c : coords){
			int x{anchor.first + c.first};
			int y{anchor.second + c.second};

			// append to vector if needed
			if(y >= static_cast<int>(field.size()))
				field.push_back({});

			// set field as occupied
			field.at(y).at(x) = true;
		}
	}

	// coords are relative to the bottom left corner of the rock structure
	std::vector<std::pair<int, int>> coords;

	// width and height for quick-checking the bounds
	int width;
	int height;

	// position of the whole rock represented by an anchor coordinate.
	// This also is the coordinate to which the other ones are relative to.
	std::pair<int,int> anchor;
};

// more a helper function for visualizing the field
void print_field(const field_t& field)
{
	for(int i{static_cast<int>(field.size())-1}; i >= 0; i--) {
		for(const auto& x : field.at(i)) {
			if(x)
				std::cout << "#";
			else
				std::cout << ".";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// simulates the whole procedure until rock_count rocks fell
auto solve_slow(unsigned long rock_count, std::array<FallingRock,5> rocks, std::vector<Flow> flows)
{
	field_t field{};

	std::array<FallingRock,5>::size_type r{0};
	auto rock{rocks[r]};
	rock.set_anchor(field);
	std::vector<Flow>::size_type f{0};

	for(unsigned long i{0}; i < rock_count;)
	{
		// pushed by gas
		rock.move_right_left(field, flows[f].dir);
		// std::cout << "Anchor:" << rock.anchor.first << "|" << rock.anchor.second << std::endl;
		// std::cout << flows[f] << "\n" << std::endl;
		f = (f+1) % flows.size();

		// move down
		if(!rock.move_down(field)){
			// solid
			rock.make_solid(field);
			i++;
			// next rock
			r = (r+1) % rocks.size();
			rock = rocks[r];
			rock.set_anchor(field);
			// std::cout << "Anchor:" << rock.anchor.first << "|" << rock.anchor.second << std::endl;
		}
	}
	return field.size();
}

// TODO remvoe this in the next commit
// does not work (yet). Idea was to reduce the field by dropping bottom rows
// (field_t must support pop_front) now and then -> build some sort of cache to
// try and detect cycles
// int field_reduce(field_t& field, bool print)
// {
// 	std::array<int,7> maxes{};
// 	auto i{field.size()};
// 	for(auto iter{field.rbegin()}; iter != field.rend(); iter++, i--)
// 	{
// 		if(print)
// 			std::cout << i << std::endl;
// 		unsigned long j{0};
// 		for(const auto& jter : *iter)
// 		{
// 			if(jter && maxes[j] == 0)
// 				maxes[j] = static_cast<int>(i);
// 			j++;
// 		}
// 	}
// 	int min{*std::min_element(maxes.begin(), maxes.end())};
//
// 	for(int i{0}; i < min-1; i++)
// 		field.pop_front();
// 	return min;
// }
// auto solve_mid(unsigned long rock_count, std::array<FallingRock,5> rocks, std::vector<Flow> flows)
// {
// 	field_t field{};
// 	size_t field_offset{0};
// 	std::map<field_t, std::map<int, std::map<int, std::tuple<field_t,int,size_t>>>> cache{};
//
// 	std::array<FallingRock,5>::size_type r{0};
// 	auto rock{rocks[r]};
// 	rock.set_anchor(field);
// 	std::vector<Flow>::size_type f{0};
//
// 	for(unsigned long i{0}; i < rock_count;)
// 	{
// 		// pushed by gas
// 		rock.move_right_left(field, flows[f].dir);
// 		// std::cout << "Anchor:" << rock.anchor.first << "|" << rock.anchor.second << std::endl;
// 		// std::cout << flows[f] << "\n" << std::endl;
// 		f = (f+1) % flows.size();
//
// 		// move down
// 		if(!rock.move_down(field)){
// 			// solid
// 			rock.make_solid(field);
// 			if(i == -1){
// 				field_reduce(field, true); // todo offset
// 				std::cout << i << std::endl;
// 				print_field(field);
// 			} else {
// 				std::cout << i << std::endl;
// 				field_reduce(field, false); // todo offset
// 				print_field(field);
// 			}
// 			i++;
// 			if(i % 1000000 == 0)
// 				std::cout << i << std::endl;
// 			// print_field(field);
// 			// next rock
// 			r = (r+1) % rocks.size();
// 			rock = rocks[r];
// 			rock.set_anchor(field);
// 			// std::cout << "Anchor:" << rock.anchor.first << "|" << rock.anchor.second << std::endl;
// 		}
// 	}
// 	return field.size();
// }

// trys to find a cycle in the procedure -> no need to simulate everything =>
// faster (and less memory)
auto solve_fast_general(std::array<FallingRock,5> rocks, std::vector<Flow> flows)
{
	// GENERAL NOTE: roundNr is the number of rocks that already has been fallen

	// GENERAL IDEA:
	// from https://www.reddit.com/r/adventofcode/comments/znykq2/comment/j0vj0l2/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	// (even though the linked python code didn't work for my input (or the
	// example) the idea is nice and works out)
	//
	// Terminology:
	//  - roundNr: number of the round. A round starts with creating the rock
	//    and terminates with making the rock solid
	//  - rockIdx: index of the rock (in the rocks array) which currently is
	//    falling
	//  - flowIdx: index of the flow (in the flows vector) which currently is
	//    being executed
	//
	// We assume we have recorded something like this
	//
	//           l    k   current step in simulation
	//           v    v   v
	//   [Prefix | X2 | X1]
	//   where X2 == X1
	//
	// Now if above sequence is the sequence of the x-positions the rocks became
	// solid in the corresponding round in the single element the shape of the
	// 7-piles is already encoded (try not to think about the cycle for now
	// since currently we just recorded the sequence without any cycle).
	//
	// Now if the state at X2[-1] is exactly the same as at X1[-1] we have found
	// a cycle. Let's think about constraints for this equality of states:
	//  - same rockIdx
	//  - same flowIdx
	//  - same pile structure (determines when the rock will become solid and
	//    what movements can be executed). Can be indirectly confirmed by
	//    checking if the beheviour (can be identified by the positions where
	//    the rocks became solid) is exactly the same.
	//
	// So we store the roundNr which we got from a certain combination of
	// rockIdx and flowIdx and check if the behaviour in between was the same.
	// If so, now after we encountered the same rock and the same flow the same
	// sequence will happen now as the behaviour has been repeating as well (so
	// as far as it matters the 7-piles were the same).
	//
	// We're gonna store different mappings:
	// - (roundNr) -> height
	std::vector<long> heights{};
	// - (roundNr) -> x-coord of the rock anchor when the rock became solid
	std::vector<int> xpos{};
	// - (rockIdx x flowIdx) -> roundNrs after the rock became static
	std::map<std::pair<unsigned int, unsigned int>, std::vector<long>> rounds{};

	// the field of the static rocks
	field_t field{};

	// current falling rock
	std::array<FallingRock,5>::size_type r{0};
	auto rock{rocks[r]};
	rock.set_anchor(field);

	// current flow
	std::vector<Flow>::size_type f{0};

	// run until a cycle was found
	for(long i{0}; true;)
	{
		// pushed by gas
		rock.move_right_left(field, flows[f].dir);
		f = (f+1) % flows.size(); // next flow

		// move down
		if(!rock.move_down(field)){
			// down movement wasn't possible -> make solid
			rock.make_solid(field);

			// print_field(field);

			// store stuff for cycle detection
			heights.push_back(static_cast<long>(field.size()));
			xpos.push_back(rock.anchor.first);

			// (rock x flow) -> roundNr
			auto idx{std::make_pair(r,f)};
			if(!rounds.contains(idx))
				rounds.insert({idx, {}});
			auto& round{rounds[idx]};
			round.push_back(i);

			// cycle detection:
			//           l    k   current step in simulation
			//           v    v   v
			//   [Prefix | X2 | X1]
			//   where X2 == X1 and b,m \in rounds[(rockIdx, flowIdx)]
			// Omit the current occurence as k = current instance wouldn't
			// make much sense (X1 would be empty, see visualization above)
			for(size_t k{0}; k < rounds[idx].size()-1; k++)
			{
				// l >= k wouldn't make much sence (see visualization)
				for(size_t l{0}; l < k; l++)
				{
					// would become faster if we compared the length of the two
					// ranges first before checking the whole range for equality
					if(std::ranges::equal(xpos.cbegin() + round[l], xpos.cbegin() + round[k], xpos.cbegin() + round[k], xpos.cend()-1)) {
						// we've found cycle -> store prefix, length and height growth by cycle

						//           l    k   current step in simulation
						//           v    v   v
						//   [Prefix | X2 | X1]
						// To make things more simple, we'll simply add X2 to
						// the prefix ans start the cycle at k.
						long prefix_length{round[k]};
						// calculate the length of the X1 cycle
						long cycle_length{i - round[k]};
						// calculate by how many units the height has grown
						// during the cycle (cycle is everything simulated - prefix)
						long cycle_growth{static_cast<long>(field.size()) - heights[static_cast<unsigned long>(prefix_length)]};

						// std::cout << "prefix_length " << prefix_length << std::endl;
						// std::cout << "cycle_length " << cycle_length << std::endl;
						// std::cout << "cycle_growth " << cycle_growth << std::endl;
						// std::cout << "[";
						// for(const auto& p : heights)
						// {
						// 	std::cout << p << ", ";
						// }
						// std::cout << "]" << std::endl;

						// add the heights lookup table to be able to check the
						// height after the prefix and after the "suffix" (aka
						// unfinished cycle in the end)
						return std::make_tuple(prefix_length, cycle_length, cycle_growth, std::move(heights));
					}
				}
			}

			// next round
			i++;
			// next rock
			r = (r+1) % rocks.size();
			rock = rocks[r];
			rock.set_anchor(field);
		}
	}
	// as long as it's an endless loop we'll never get here
	throw std::runtime_error("No cycle detected");
}
auto solve_fast_specific(long rock_count, long prefix_length, long cycle_length, long cycle_growth, const std::vector<long>& heights)
{
	// how often does the cycle repeat itself?
	auto cnt_cycle{(rock_count - prefix_length) / cycle_length};

	// how many iterations are left after the last full cycle?
	auto rem_cycle{(rock_count - prefix_length) % cycle_length};

	// the "suffix" (aka the unfinished cycle in the end) starts at the begin of
	// a cycle which in turn is preceeded by the prefix. So instead of
	// calculating height[prefix-1] + height_growth[suffix-1] we can just
	// calculate height[prefix_length + suffix_length - 1]
	auto pre_and_suffix{heights[static_cast<unsigned long>(prefix_length + rem_cycle - 1)]};

	return pre_and_suffix + cnt_cycle * cycle_growth;
}

int main()
{
	// std::ifstream ifs{"../problems/day17.dat.testing"};
	std::ifstream ifs{"../problems/day17.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	std::vector<Flow> flows = aocutils::vectorize_ifs<Flow>(ifs);

	auto rocks = std::array<FallingRock, 5>{
		// ####
		FallingRock(std::vector<std::pair<int,int>>{{0,0}, {1,0}, {2,0}, {3,0}}),
		//  # 
		// ###
		//  #
		FallingRock(std::vector<std::pair<int,int>>{{1,0}, {0,1}, {1,1}, {2,1}, {1,2}}),
		//   #
		//   #
		// ###
		FallingRock(std::vector<std::pair<int,int>>{{0,0}, {1,0}, {2,0}, {2,1}, {2,2}}),
		// #
		// #
		// #
		// #
		FallingRock(std::vector<std::pair<int,int>>{{0,0}, {0,1}, {0,2}, {0,3}}),
		// ##
		// ##
		FallingRock(std::vector<std::pair<int,int>>{{0,0}, {1,0}, {0,1}, {1,1}}),
	};


	// auto part1 = solve_slow(2022, rocks, flows);
	// auto part2 = solve_slow(1000000000000, rocks, flows); // won't terminate

	auto gen{solve_fast_general(rocks, flows)};
	auto part1 = solve_fast_specific(2022, std::get<0>(gen), std::get<1>(gen), std::get<2>(gen), std::get<3>(gen));
	auto part2 = solve_fast_specific(1000000000000, std::get<0>(gen), std::get<1>(gen), std::get<2>(gen), std::get<3>(gen));

	std::cout << "Day 17:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl
			  ;
}
