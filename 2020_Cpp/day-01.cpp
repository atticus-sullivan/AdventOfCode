#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<int> readFile(const std::string fn){
	std::ifstream ifs{fn};
	if(!ifs)
		std::cout << "couldn't open '" << fn << "' for reading\n";

	std::vector<int> ret;
	std::string c;
	while(std::getline(ifs, c, '\n')){
		ret.emplace_back(std::move(std::stoi(c)));
	}
	return ret;
}

int part1(const std::vector<int>& input){
	for(auto i : input){
		for(auto j : input){
			if((i+j) == 2020){
				return i*j;
			}
		}
	}
	return -1;
}

int part2(const std::vector<int>& input){
	for(auto i : input){
		for(auto j : input){
			for(auto k : input){
				if((i+j+k) == 2020){
					return i*j*k;
				}
			}
		}
	}
	return -1;
}

int main() {
	auto f = readFile("../day-01.dat");
	std::cout << "Part1:\n" << part1(f) << "\n\n";
	std::cout << "Part2:\n" << part2(f) << "\n";
}
