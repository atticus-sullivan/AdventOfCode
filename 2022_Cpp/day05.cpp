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

struct Instruction {
	int amount;
	int src, dst;

	friend std::istream& operator>>(std::istream& is, Instruction& i) {
		is.ignore(4); // "move"
		is >> i.amount;
		// std::cout << i.amount << std::endl;
		is.ignore(5); // " from"
		is >> i.src;
		// std::cout << i.src << std::endl;
		is.ignore(3); // " to"
		is >> i.dst;
		// std::cout << i.dst << std::endl;
		is.ignore(1); // "\n"

		return is;
	}
};

struct Crates {
	std::map<int, std::deque<char>> stacks;


	void apply(const Instruction& ins, bool part2 = false){
		// if(part2){
		// 	auto dst = stacks.at(ins.dst);
		// 	auto src = stacks.at(ins.src);
		// 	auto beg = src.end()-ins.amount;
		// 	auto end = src.end();
		//
		// 	std::move(beg,end, dst.end());
		// 	src.erase(beg, end);
		// }
		// else
		// {
			for(int i=0; i < ins.amount; i++){
				if(part2){
					std::cout << "here1" << std::endl;
					stacks.at(ins.dst).push_back(stacks.at(ins.src).at(stacks.at(ins.src).size()-ins.amount+i));
					std::cout << "here2" << std::endl;
					stacks.at(ins.src).erase(stacks.at(ins.src).end()-ins.amount+i);
					std::cout << "here3" << std::endl;
				} else {
					stacks.at(ins.dst).push_back(stacks.at(ins.src).back());
					stacks.at(ins.src).pop_back();
				}
			}
		// }
	}

	std::string get_word(){
		std::string res{};
		for(const auto& [key,value]: stacks){
			res = res + value.back();
		}
		return res;
	}

	friend std::ostream& operator<<(std::ostream& os, Crates& i) {
		for(const auto& [key,value]: i.stacks){
			os << key << "-> {";
			// os << value.size();
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
					// std::cout << "created: " << j << std::endl;
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
				// std::cout << "end " << j << std::endl;
				i.stacks.at(j).push_front(c);
			}
			first = false;
			std::getline(is, l);
		}
		return is;
	}
};

std::string partA(){
	std::ifstream ifs{"../day05.dat"};
	if(ifs.fail()){
		throw std::runtime_error("File couldn't be opened!");
	}

	Crates c{};
	ifs >> c;
	std::vector<Instruction> instructions{std::istream_iterator<Instruction>{ifs}, {}};
	std::cout << instructions.size() << std::endl;

	std::cout << c;
	for(const auto& inst : instructions){
		c.apply(inst);
		std::cout << std::endl;
		std::cout << c;
	}
	return c.get_word();
}

int main(int argc, char* argv[]) {
	// std::ifstream ifs{"../day05.dat.testing"};
	std::ifstream ifs{"../day05.dat"};
	if(ifs.fail()){
		throw std::runtime_error("File couldn't be opened!");
	}

	Crates c{};
	ifs >> c;
	std::vector<Instruction> instructions{std::istream_iterator<Instruction>{ifs}, {}};
	std::cout << instructions.size() << std::endl;

	std::cout << c;
	for(const auto& inst : instructions){
		c.apply(inst, true);
		std::cout << std::endl;
		std::cout << c;
	}

	// std::string part1 = partA();
	
	std::cout << "Day 04:" << '\n'
		// << "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << c.get_word() << std::endl
		;
}
