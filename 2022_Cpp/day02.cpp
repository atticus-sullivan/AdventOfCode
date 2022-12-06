#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <numeric>

#include "aocutils.h"

enum class Outcome {
	DEFEAT = 0,
	DRAW   = 3,
	WIN    = 6,
};
Outcome outcome_parse(char x){
	using enum Outcome;
	switch (x) {
		case 'X': return DEFEAT;
		case 'Y': return DRAW;
		case 'Z': return WIN;
	}
	throw std::runtime_error("Invalid character read");
}
std::string outcome_str(Outcome x){
	using enum Outcome;
	switch (x) {
		case DEFEAT: return "Defeat";
		case DRAW:   return "Draw";
		case WIN:    return "Win";
	}
	throw std::runtime_error("Invalid outcome");
}

enum class Play {
	ROCK     = 1,
	PAPER    = 2,
	SCISSORS = 3,
};
Play play_parse(char x){
	using enum Play;
	switch (x) {
		case 'A':
		case 'X': return ROCK;
		case 'B':
		case 'Y': return PAPER;
		case 'C':
		case 'Z': return SCISSORS;
	}
	throw std::runtime_error("Invalid character read");
}
std::string play_str(Play x){
	using enum Play;
	switch (x) {
		case ROCK:     return "Rock";
		case PAPER:    return "Paper";
		case SCISSORS: return "Scissors";
	}
	throw std::runtime_error("Invalid play");
}

struct Rucksack {
	Play opponent;
	Play ownA;
	Play ownB;
	int pointsA;
	int pointsB;

	int calc_points(Play opponent, Play own){
		bool win =
			own == Play::ROCK     && opponent == Play::SCISSORS ||
			own == Play::PAPER    && opponent == Play::ROCK ||
			own == Play::SCISSORS && opponent == Play::PAPER
			;
		if(win)
			return static_cast<int>(Outcome::WIN) + static_cast<int>(own);
		if(opponent == own)
			return static_cast<int>(Outcome::DRAW) + static_cast<int>(own);
		return static_cast<int>(Outcome::DEFEAT) + static_cast<int>(own);
	}
	
	Play calc_own(Outcome out, Play opponent){
		if(out == Outcome::WIN){
			if(opponent == Play::ROCK)     return Play::PAPER;
			if(opponent == Play::PAPER)    return Play::SCISSORS;
			if(opponent == Play::SCISSORS) return Play::ROCK;
		}
		else if(out == Outcome::DRAW){
			return opponent;
		}
		else if(out == Outcome::DEFEAT){
			if(opponent == Play::ROCK)     return Play::SCISSORS;
			if(opponent == Play::PAPER)    return Play::ROCK;
			if(opponent == Play::SCISSORS) return Play::PAPER;
		}
		throw std::runtime_error("Invalid outcome");
	}

	friend std::istream& operator>>(std::istream& is, Rucksack& i) {
		i = std::move(Rucksack{});
		std::string l;
		std::getline(is, l);
		if(l != ""){
			i.opponent = play_parse(l.at(0));
			i.ownA     = play_parse(l.at(2));
			i.pointsA  = i.calc_points(i.opponent, i.ownA);
			i.ownB     = i.calc_own(outcome_parse(l.at(2)), i.opponent);
			i.pointsB  = i.calc_points(i.opponent, i.ownB);
		}
		return is;
	}
};

int main(int argc, char* argv[]) {
	// std::ifstream ifs{"../problems/day02.dat.testing"};
	std::ifstream ifs{"../problems/day02.dat"};
	if(ifs.fail()){
		throw std::runtime_error("File couldn't be opened!");
	}
	std::vector<Rucksack> games = aocutils::vectorize_ifs<Rucksack>(ifs);

	auto part1 = std::accumulate(games.begin(), games.end(), 0, [](int b, const auto& r){return r.pointsA+b;});
	auto part2 = std::accumulate(games.begin(), games.end(), 0, [](int b, const auto& r){return r.pointsB+b;});

	std::cout << "Day 02:" << '\n'
		<< "  Part 1: " << part1 << std::endl
		<< "  Part 2: " << part2 << std::endl
		;
}
