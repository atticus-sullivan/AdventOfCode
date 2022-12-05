#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <numeric>
#include <cassert>
#include "aocutils.h"

struct Instruction {
	int amount;
	int src, dst;

	friend std::istream& operator>>(std::istream& is, Instruction& i) {
		i = std::move(Instruction{});
		is.ignore(4); // "move"
		is >> i.amount;
		is.ignore(5); // " from"
		is >> i.src;
		is.ignore(3); // " to"
		is >> i.dst;
		is.ignore(1); // "\n"

		return is;
	}
};

struct Crates {
	std::map<int, std::deque<char>> stacks;


	void apply(const Instruction& ins, bool part2 = false){
		auto& dst = stacks.at(ins.dst);
		auto& src = stacks.at(ins.src);
		if(part2){
			auto beg = src.end()-ins.amount;
			auto end = src.end();

			// std::move(beg, end, std::back_inserter(dst));
			dst.insert(dst.end(), std::make_move_iterator(beg),
					std::make_move_iterator(end));
			src.erase(beg, end);
		} else {
			dst.insert(
					dst.end(), std::make_move_iterator(src.rbegin()),
					std::make_move_iterator(src.rbegin() + ins.amount));
			src.erase(src.end() - ins.amount, src.end());
		}
	}

	std::string get_word(){
		std::ostringstream output{};
		std::string res{};
		for(const auto& [key,value]: stacks){
			output << value.back();
		}
		return output.str();
	}

	friend std::ostream& operator<<(std::ostream& os, Crates& i) {
		for(const auto& [key,value]: i.stacks){
			os << key << "-> {";
			for(const auto& ele : value){
				os << ele << ", ";
			}
			os << "}\n";
		}
		os << std::endl;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Crates& i) {
		std::string l;
		std::getline(is, l);
		bool first = true;
		while(l != ""){
			for(int j=1; true; j++){
				if(l.length() <= 4*(j-1)+1) break;
				if(first){
					i.stacks.insert({j, {}});
				}
				// 1, 5, 9
				char c = l.at(4*(j-1)+1);
				if(c == ' '){
					assert(i.stacks.at(j).size() == 0 && "Stack has to be empty if current line is empty");
					continue;
				}
				if('0' <= c && c <= '9'){
					int k = c - '0';
					assert(k == j && "Index not matching the footer");
					continue;
				}
				i.stacks.at(j).push_front(c);
			}
			first = false;
			std::getline(is, l);
		}
		return is;
	}
};

int main(int argc, char* argv[]) {
	// std::ifstream ifs{"../day05.dat.testing"};
	std::ifstream ifs{"../day05.dat"};
	if(ifs.fail()){
		throw std::runtime_error("File couldn't be opened!");
	}

	Crates cA{};
	ifs >> cA;
	std::vector<Instruction> instructions = aocutils::vectorize_ifs<Instruction>(ifs);

	Crates cB = cA; // copy construct to work on same setup again later

	for(const auto& inst : instructions){
		cA.apply(inst);
	}
	std::string part1 = cA.get_word();

	for(const auto& inst : instructions){
		cB.apply(inst, true);
	}
	std::string part2 = cB.get_word();

	std::cout << "Day 04:" << '\n'
		<< "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << part2 << std::endl
		;
}
