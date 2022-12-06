#include <ranges>
#include <vector>
#include <concepts>
#include <cassert>
#include <algorithm>

#pragma once

namespace aocutils
{
// see
// https://stackoverflow.com/questions/74662715/cpp-use-istream-iterator-to-populate-a-vector-avoid-copy
template<class T>
	requires std::movable<T>
	std::vector<T> vectorize_ifs(std::ifstream& ifs)
	{
		std::vector<T> ret{};
		std::ranges::move(std::views::istream<T>(ifs), std::back_inserter(ret));
		return ret;
	}

template<class T>
	requires std::totally_ordered<T>
	struct Range
	{
		T lower, upper;
		Range() = default;
		Range(int lower, int upper): lower{lower}, upper{upper} {
			assert(lower <= upper && "lower <= upper");
		}

		bool contains(Range other){
			return
				(lower <= other.lower && upper >= other.upper) ||
				(other.lower <= lower && other.upper >= upper);
		}
		bool overlap(Range other){
			bool r =
				(lower <= other.lower && upper < other.lower) ||
				(other.lower <= lower && other.upper < lower);
			return !r;
		}
	};

} // Namespace aocutils
