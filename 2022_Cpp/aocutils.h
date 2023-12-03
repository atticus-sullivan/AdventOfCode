#include <ranges>
#include <vector>
#include <concepts>
#include <cassert>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>

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

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

///////////
// GRAPHS //
///////////
// TODO make it work without copying -> just references in Neighbours -> just
// doesn't work if T is just indices...

// leave it up to the user which data-structures are being used for storing
// visited, distance and parent (might be an external vector/map or stored just
// as a member of T)
//
// works in directed, weighted (>=0) graph
template <typename T>
void dijkstra(
		// distance should be inf for all
		std::function<void(T&, int)> set_distance, std::function<int(const T&)> get_distance,
		// visited should be false for all
		std::function<void(T&, bool)> set_visited, std::function<bool(const T&)> get_visited,
		std::function<void(T&, const T&)> set_parent,
		// TODO return T as reference. Problem: 2022day12 T is just an index to an array => no long-live object to refer to
		std::function<std::vector<std::pair<T,int>>(const T&)> neighbours,
		T& start
		)
{
	auto comp = [](const std::pair<int,T>& a, const std::pair<int,T>& b){return a.first > b.first;};
	std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T>>, decltype(comp)> pq{};
	pq.emplace(0,start);

	set_distance(start, 0);
	while(pq.size() > 0)
	{
		auto [_,v] = pq.top();
		pq.pop();
		// check if v is already processed (because of ust inserting new values
		// to the pq instead of dooing an uüdate)
		if(get_visited(v)) continue;
		set_visited(v, true);

		for(auto& [w,c] : neighbours(v))
		{
			int dist = get_distance(v) + c;
			if(dist < get_distance(w))
			{
				set_parent(w, v);
				pq.push(std::make_pair(dist,w));
				set_distance(w, dist);
			}
		}
	}
}

// works in directed, acyclic, unweighted graph
template <typename T>
std::optional<std::reference_wrapper<T>> breath_first_search(
		std::function<void(T&, bool)> set_visited, std::function<bool(const T&)> get_visited,
				std::function<void(T&, const T&)> set_parent,
				std::function<std::vector<std::pair<T,int>>(const T&)> neighbours,
				std::function<bool(const T&)> pred,
				T& start)
{
	std::queue<T> q{};
	q.push(start);
	while(!q.empty())
	{
		T &v = q.front();
		q.pop();
		if(pred(v)) return {v};

		for(auto [w,c] : neighbours(v))
		{
			assert(c == 1 && "Graph must be unweighted");
			if(!get_visited(w))
			{
				set_visited(w, true);
				set_parent(w, v);
				q.push(w);
			}
		}
	}
	return {};
}

	template <typename T1, typename T2>
bool is_disjoint(const std::set<T1> &a, const std::set<T2> &b)
{
	auto it_a = a.begin();
	auto it_b = b.begin();
	while (it_a != a.end() && it_b != b.end())
	{
		if(*it_a == *it_b)
			return false;
		if(*it_a < *it_b)
			it_a = std::lower_bound(++it_a, a.end(), *it_b);
		else if(*it_a > *it_b)
			it_b = std::lower_bound(++it_b, b.end(), *it_a);
	}
	return true;
}


} // Namespace aocutils
