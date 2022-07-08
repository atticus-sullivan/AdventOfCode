#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

std::vector<std::string> readFile(const std::string fn){
	std::ifstream ifs{fn};
	if(!ifs)
		std::cout << "couldn't open '" << fn << "' for reading\n";

	std::vector<std::string> ret;
	std::string c;
	while(std::getline(ifs, c, '\n')){
		ret.emplace_back(std::move(c));
	}
	return ret;
}

int part1(const std::vector<std::string>& input){
	int ret{0};
	for(std::string i : input){
		std::regex pat {R"(([0-9]+)-([0-9]+) (.): (.*))"};
		std::smatch p;
		std::regex_search(i, p, pat);
		if(p.size() != 5){
			std::cout << "Error: not 5 submatches found in line \"" << i << "\"\n";
			continue;
		}
		int min = std::stoi(p[1]);
		int max = std::stoi(p[2]);
		char c = p[3].str()[0];
		std::string str{p[4]};
		
		int count = std::count(str.begin(), str.end(), c);
		if(count >= min && count <= max) ++ret;
	}
	return ret;
}

int part2(const std::vector<std::string>& input){
	int ret{0};
	for(std::string i : input){
		std::regex pat {R"(([0-9]+)-([0-9]+) (.): (.*))"};
		std::smatch p;
		std::regex_search(i, p, pat);
		if(p.size() != 5){
			std::cout << "Error: not 5 submatches found in line \"" << i << "\"\n";
			continue;
		}
		int min = std::stoi(p[1])-1;
		int max = std::stoi(p[2])-1;
		char c = p[3].str()[0];
		std::string str{p[4]};
		
		if((str[min] == c && str[max] != c) || (str[min] != c && str[max] == c))
			++ret;
	}
	return ret;
}

int main() {
	auto f = readFile("../day-02.dat");
	std::cout << "Part1:\n" << part1(f) << "\n\n";
	std::cout << "Part2:\n" << part2(f) << "\n";
}
