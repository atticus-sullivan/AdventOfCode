#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "aocutils.h"

enum FieldEle
{
	WALL,
	PATH,
};

enum Dir
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
};

using Pos      = std::pair<int, int>;
using Field    = std::map<Pos, FieldEle>;
using Mov_part = std::map<int, std::pair<int, int>>;

using Rot2 = std::map<std::pair<Pos, Dir>, int>;
using Mov2 = std::map<std::pair<Pos, Dir>, Pos>;

struct Mima
{
	Mov_part mov_x;
	Mov_part mov_y;

	Mov2 mov;
	Rot2 rot;
};

struct Map
{
	Field field;
	Mima  mima;
};

struct State
{
	Pos pos;
	Dir dir;
};

void print_map(std::ostream &os, const Map &map, const State &state)
{
	auto min_y = std::min_element(map.mima.mov_y.begin(), map.mima.mov_y.end(),
								  [](const auto &l, const auto &r)
								  { return l.second.first < r.second.first; });
	for(int y{0}; y >= min_y->second.first; y--)
	{
		for(int x{0}; x <= map.mima.mov_x.find(y)->second.second; x++)
		{
			if(std::make_pair(x, y) == state.pos)
			{
				os << "\033[41m"
				   << "o"
				   << "\033[0m";
				continue;
			}
			if(map.field.contains({x, y}))
			{
				if(map.field.find({x, y})->second == WALL) os << '#';
				else os << '.';
			}
			else { os << ' '; }
		}
		os << '|' << std::endl;
	}
}

struct Instruction
{
	virtual void execute(State &state, const Map &map) = 0;
};

struct Walk : Instruction
{
	int amount;

	Walk(int &a) : amount{a} {}

	void execute(State &state, const Map &map) override
	{
		// std::cout << "walk " << state.dir << std::endl;
		for(auto i{0}; i < this->amount; i++)
		{
			// new position
			Pos p2{state.pos};
			Dir dir{state.dir};
			switch(state.dir)
			{
				case NORTH:
					p2.second++;
					break;
				case SOUTH:
					p2.second--;
					break;
				case EAST:
					p2.first++;
					break;
				case WEST:
					p2.first--;
					break;
				default:
					throw std::runtime_error("Invalid dir");
			}
			// std::cout << "check " << p2.first << "|" << p2.second <<
			// std::endl; wrap arround
			if(!map.field.contains(p2))
			{
				if(!map.mima.rot.contains({p2, state.dir}))
					throw std::runtime_error("rot not found");
				dir = static_cast<Dir>(
					(static_cast<int>(state.dir) +
					 map.mima.rot.find({p2, state.dir})->second) %
					4);
				if(!map.mima.mov.contains({p2, state.dir}))
					throw std::runtime_error("mov not found");
				p2 = map.mima.mov.find({p2, state.dir})->second;
				// std::cout << "\033[31mwrap\033[0m to " << p2.first << "|" <<
				// p2.second << std::endl;
			}
			// check for wall
			if(!map.field.contains(p2))
				throw std::runtime_error("p2 still not in field");
			if(map.field.find(p2)->second == WALL)
			{
				// std::cout << "hit wall" << std::endl;
				return;
			}
			else
			{
				state.pos = p2;
				state.dir = dir;
			}
		}
	}
};

struct Turn : Instruction
{
	bool left;

	Turn(bool l) : left{l} {}

	void execute(State &state, const Map &map) override
	{
		if(left)
			state.dir = static_cast<Dir>((static_cast<int>(state.dir) + 3) % 4);
		else
			state.dir = static_cast<Dir>((static_cast<int>(state.dir) + 1) % 4);
	}
};

struct Game
{
	Map                                       map;
	std::vector<std::unique_ptr<Instruction>> instructions;

	friend std::istream &operator>>(std::istream &is, Game &i)
	{
		is >> std::noskipws;
		// default initialize the most probable moved from object
		i = std::move(Game{});

		int  x{0}, y{0};
		char c{};
		bool prev_nl{false};
		while(true)
		{
			is >> c;
			if(c == '\n')
			{
				y--;
				x = 0;
				if(prev_nl) { break; }
				prev_nl = true;
				continue;
			}
			prev_nl = false;
			if(c == ' ')
			{
				x++;
				continue;
			}

			if(c == '#') { i.map.field[{x, y}] = WALL; }
			if(c == '.') { i.map.field[{x, y}] = PATH; }

			if(!i.map.mima.mov_y.contains(x)) i.map.mima.mov_y[x] = {0, y};
			if(!i.map.mima.mov_x.contains(y)) i.map.mima.mov_x[y] = {x, 0};

			i.map.mima.mov_x[y].second = x;
			i.map.mima.mov_y[x].first  = y;
			x++;
		}

		int  amount{};
		char dir{};
		while(!is.eof())
		{
			is >> amount;
			if(is.eof()) break;
			i.instructions.push_back(std::make_unique<Walk>(amount));
			is >> dir;
			if(dir == '\n' || is.eof()) break;
			i.instructions.push_back(std::make_unique<Turn>(dir == 'L'));
		}
		is >> c;

		return is;
	}
	friend std::ostream &operator<<(std::ostream &os, const Game &i)
	{
		auto min_y =
			std::min_element(i.map.mima.mov_y.begin(), i.map.mima.mov_y.end(),
							 [](const auto &l, const auto &r)
							 { return l.second.first < r.second.first; });
		for(int y{0}; y >= min_y->second.first; y--)
		{
			for(int x{0}; x <= i.map.mima.mov_x.find(y)->second.second; x++)
			{
				if(i.map.field.contains({x, y}))
				{
					if(i.map.field.find({x, y})->second == WALL) os << '#';
					else os << '.';
				}
				else { os << ' '; }
			}
			os << '|' << std::endl;
		}
		return os;
	}
};

int solve(Game &game)
{
	State state{
		{game.map.mima.mov_x[0].first, 0},
        EAST
    };
	for(const auto &i : game.instructions)
	{
		i->execute(state, game.map);
		// print_map(std::cout, game.map, state);
		// std::cout << std::endl;
	}

	return 1000 * (-state.pos.second + 1) + 4 * (state.pos.first + 1) +
		   ((static_cast<int>(state.dir) + 3) % 4);
}

int solve1(Game &game)
{
	game.map.mima.mov.clear();
	game.map.mima.rot.clear();

	for(const auto &[x, mima] : game.map.mima.mov_y)
	{
		game.map.mima.mov[{
			{x, mima.first - 1},
            SOUTH
        }] = {x, mima.second};
		game.map.mima.rot[{
			{x, mima.first - 1},
            SOUTH
        }] = 0;

		game.map.mima.mov[{
			{x, mima.second + 1},
            NORTH
        }] = {x, mima.first};
		game.map.mima.rot[{
			{x, mima.second + 1},
            NORTH
        }] = 0;
	}

	for(const auto &[y, mima] : game.map.mima.mov_x)
	{
		game.map.mima.mov[{
			{mima.first - 1, y},
            WEST
        }] = {mima.second, y};
		game.map.mima.rot[{
			{mima.first - 1, y},
            WEST
        }] = 0;

		game.map.mima.mov[{
			{mima.second + 1, y},
            EAST
        }] = {mima.first, y};
		game.map.mima.rot[{
			{mima.second + 1, y},
            EAST
        }] = 0;
	}

	return solve(game);
}

int solve2(Game &game, bool testing)
{
	game.map.mima.mov.clear();
	game.map.mima.rot.clear();

	if(testing)
	{
		// testing
		for(int y{0}; y <= 3; y++)
		{
			// light green
			game.map.mima.mov[{
				{12, 0 - y},
                EAST
            }] = {15, -11 + y};
			game.map.mima.rot[{
				{12, 0 - y},
                EAST
            }] = 2;

			game.map.mima.rot[{
				{16, -11 + y},
                EAST
            }] = 2;
			game.map.mima.mov[{
				{16, -11 + y},
                EAST
            }] = {11, 0 - y};
		}
		for(int y{0}; y <= 3; y++)
		{
			// gray
			game.map.mima.mov[{
				{7, 0 - y},
                WEST
            }] = {4 + y, -4};
			game.map.mima.rot[{
				{7, 0 - y},
                WEST
            }] = 3;

			game.map.mima.mov[{
				{4 + y, -3},
                NORTH
            }] = {8, 0 - y};
			game.map.mima.rot[{
				{4 + y, -3},
                NORTH
            }] = 1;
		}
		for(int y{0}; y <= 3; y++)
		{
			// red
			game.map.mima.mov[{
				{12, -4 - y},
                EAST
            }] = {15 - y, -8};
			game.map.mima.rot[{
				{12, -4 - y},
                EAST
            }] = 1;

			game.map.mima.mov[{
				{15 - y, -7},
                NORTH
            }] = {11, -4 - y};
			game.map.mima.rot[{
				{15 - y, -7},
                NORTH
            }] = 3;
		}
		for(int y{0}; y <= 3; y++)
		{
			// light blue
			game.map.mima.mov[{
				{-1, -4 - y},
                SOUTH
            }] = {15 - y, -11};
			game.map.mima.rot[{
				{-1, -4 - y},
                SOUTH
            }] = 3;

			game.map.mima.mov[{
				{15 - y, -12},
                WEST
            }] = {0, -4 - y};
			game.map.mima.rot[{
				{15 - y, -12},
                WEST
            }] = 1;
		}
		for(int y{0}; y <= 3; y++)
		{
			// orange
			game.map.mima.mov[{
				{7, -8 - y},
                SOUTH
            }] = {7 - y, -7};
			game.map.mima.rot[{
				{7, -8 - y},
                SOUTH
            }] = 3;

			game.map.mima.mov[{
				{7 - y, -8},
                WEST
            }] = {8, -8 - y};
			game.map.mima.rot[{
				{7 - y, -8},
                WEST
            }] = 1;
		}
		for(int x{0}; x <= 3; x++)
		{
			// blue
			game.map.mima.mov[{
				{8 + x, 1},
                NORTH
            }] = {3 - x, -4};
			game.map.mima.rot[{
				{8 + x, 1},
                NORTH
            }] = 2;

			game.map.mima.mov[{
				{3 - x, -3},
                NORTH
            }] = {8 + x, 0};
			game.map.mima.rot[{
				{3 - x, -3},
                NORTH
            }] = 2;
		}
		for(int x{0}; x <= 3; x++)
		{
			// green
			game.map.mima.mov[{
				{0 + x, -8},
                SOUTH
            }] = {11 - x, -11};
			game.map.mima.rot[{
				{0 + x, -8},
                SOUTH
            }] = 2;

			game.map.mima.mov[{
				{11 - x, -12},
                SOUTH
            }] = {0 + x, -7};
			game.map.mima.rot[{
				{11 - x, -12},
                SOUTH
            }] = 2;
		}
	}
	else
	{
		for(int y{0}; y <= 49; y++)
		{
			// light green
			game.map.mima.mov[{
				{150, 0 - y},
                EAST
            }] = {99, -149 + y};
			game.map.mima.rot[{
				{150, 0 - y},
                EAST
            }] = 2;

			game.map.mima.rot[{
				{100, -149 + y},
                EAST
            }] = 2;
			game.map.mima.mov[{
				{100, -149 + y},
                EAST
            }] = {149, 0 - y};
		}
		for(int y{0}; y <= 49; y++)
		{
			// gray
			game.map.mima.mov[{
				{100, -50 - y},
                EAST
            }] = {100 + y, -49};
			game.map.mima.rot[{
				{100, -50 - y},
                EAST
            }] = 3;

			game.map.mima.rot[{
				{100 + y, -50},
                SOUTH
            }] = 1;
			game.map.mima.mov[{
				{100 + y, -50},
                SOUTH
            }] = {99, -50 - y};
		}
		for(int y{0}; y <= 49; y++)
		{
			// orange
			game.map.mima.mov[{
				{50, -150 - y},
                EAST
            }] = {50 + y, -149};
			game.map.mima.rot[{
				{50, -150 - y},
                EAST
            }] = 3;

			game.map.mima.rot[{
				{50 + y, -150},
                SOUTH
            }] = 1;
			game.map.mima.mov[{
				{50 + y, -150},
                SOUTH
            }] = {49, -150 - y};
		}
		for(int y{0}; y <= 49; y++)
		{
			// red
			game.map.mima.mov[{
				{49, -50 - y},
                WEST
            }] = {0 + y, -100};
			game.map.mima.rot[{
				{49, -50 - y},
                WEST
            }] = 3;

			game.map.mima.rot[{
				{0 + y, -99},
                NORTH
            }] = 1;
			game.map.mima.mov[{
				{0 + y, -99},
                NORTH
            }] = {50, -50 - y};
		}
		for(int y{0}; y <= 49; y++)
		{
			// pink
			game.map.mima.mov[{
				{-1, -150 - y},
                WEST
            }] = {50 + y, 0};
			game.map.mima.rot[{
				{-1, -150 - y},
                WEST
            }] = 3;

			game.map.mima.rot[{
				{50 + y, 1},
                NORTH
            }] = 1;
			game.map.mima.mov[{
				{50 + y, 1},
                NORTH
            }] = {0, -150 - y};
		}
		for(int y{0}; y <= 49; y++)
		{
			// green
			game.map.mima.mov[{
				{-1, -100 - y},
                WEST
            }] = {50, -49 + y};
			game.map.mima.rot[{
				{-1, -100 - y},
                WEST
            }] = 2;

			game.map.mima.rot[{
				{49, -49 + y},
                WEST
            }] = 2;
			game.map.mima.mov[{
				{49, -49 + y},
                WEST
            }] = {0, -100 - y};
		}
		for(int x{0}; x <= 49; x++)
		{
			// light blue
			game.map.mima.mov[{
				{100 + x, 1},
                NORTH
            }] = {0 + x, -199};
			game.map.mima.rot[{
				{100 + x, 1},
                NORTH
            }] = 0;

			game.map.mima.rot[{
				{0 + x, -200},
                SOUTH
            }] = 0;
			game.map.mima.mov[{
				{0 + x, -200},
                SOUTH
            }] = {100 + x, 0};
		}
	}

	return solve(game);
}

int main()
{
	// std::ifstream ifs{"../problems/day22.dat.testing"};
	std::ifstream ifs{"../problems/day22.dat"};
	if(ifs.fail()) throw std::runtime_error("File couldn't be opened!");

	Game game{};
	ifs >> game;

	auto part1 = solve1(game);
	auto part2 = solve2(game, false);
	std::cout
		<< "Attention: Part2 is only solved for the specific input. No automatic calculation how the cube must be folded!"
		<< std::endl;

	std::cout << "Day 22:" << '\n'
			  << "  Part 1: " << part1 << std::endl
			  << "  Part 2: " << part2 << std::endl;
}
