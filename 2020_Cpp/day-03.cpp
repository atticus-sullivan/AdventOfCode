#include <iostream>
#include <fstream>
#include <string>
#include <vector>
/* #include <regex> */
/* #include <algorithm> */

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
	return 0;
}

int part2(const std::vector<std::string>& input){
	return 0;
}

int main() {
	auto f = readFile("../problems/day-03.dat");
	std::cout << "Part1:\n" << part1(f) << "\n\n";
	std::cout << "Part2:\n" << part2(f) << "\n";
}
