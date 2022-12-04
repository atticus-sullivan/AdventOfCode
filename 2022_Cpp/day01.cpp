#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <numeric>

struct Game {
	std::vector<int> bag;
	int calories;

	Game() {
		bag = {};
		calories = 0;
		bag.clear();
		// std::cout << bag.size() << std::endl;
		// std::cout << this << std::endl;
	}

	operator int(){return calories;}

	friend std::istream& operator>>(std::istream& is, Game& i) {
		// std::cout << &i << std::endl;
		// std::cout << i.bag.size() << std::endl;
		i.bag.clear();
		std::string l;
		std::getline(is, l);
		while(l != ""){
			// std::cout << "'" << l << "'" << std::endl;
			i.bag.push_back(std::stoi(l));
			std::getline(is, l);
		}
		i.calories = std::reduce(i.bag.begin(), i.bag.end());
		return is;
	}
};

int main(int argc, char* argv[]) {
	// std::ifstream ifs{argv[1]};
	std::ifstream ifs{"../day01.dat"};
	if(ifs.fail()){
		throw std::runtime_error("File couldn't be opened!");
	}
	// https://en.cppreference.com/w/cpp/iterator/istream_iterator
	/* std::istream_iterator is a single-pass input iterator that reads successive
	 * objects of type T from the std::basic_istream object for which it was
	 * constructed, by calling the appropriate operator>>.
	 */
	auto tmp = std::istream_iterator<Game>{ifs};
	// Als 2. Parameter wird ein neuer istream_iterator initialisiert ohne Argumente welcher dann ein End-of-stream iterator ist.
	std::vector<Game> elves{tmp, {}};
	// for (const auto& ele : elves) {
	// 	std::cout << ele.calories << std::endl;
	// 	for(const auto& ele2 : ele.bag){
	// 		std::cout << "  " << ele2 << std::endl;
	// 	}
	// }
	std::sort(elves.begin(), elves.end(), [](const auto& a, const auto& b) {return a.calories < b.calories;});
	auto part2 = std::accumulate(elves.end()-3, elves.end(), 0);
	auto x = elves.end()-1;
	// old stuff before the need of sorting the vector
	// auto x = std::ranges::max_element(elves.begin(), elves.end(), [](const auto& a, const auto& b) {return a.calories < b.calories;});

	std::cout << "Day 01:" << '\n'
		<< "  Part 1: " << x->calories << std::endl
		<< "  Part 2: " << part2 << '\n'
		;
}
