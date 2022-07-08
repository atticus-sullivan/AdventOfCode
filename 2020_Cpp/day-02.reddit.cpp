#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

struct pass_info {
  bool part1, part2;

  void assign (std::string const& line) {
    int lo, hi;
    char c;
    if (int idx; 3 == sscanf(line.c_str(), "%d-%d %c: %n", &lo, &hi, &c, &idx)) {
      std::string_view pass {line.begin() + idx, line.end()};
      auto n = std::ranges::count (pass, c);
      part1 = lo <= n && n <= hi;
      part2 = (pass[lo - 1] == c) ^ (pass[hi - 1] == c);
    }
  }
  
  friend std::istream& operator>> (std::istream& is, pass_info& i) {
    std::string l;
    std::getline (is, l);
    i.assign(l);
    return is;
  }
};

int main (int argc, char* argv[]) {
  std::ifstream ifs{argv[1]};
  std::vector<pass_info> data{std::istream_iterator<pass_info>{ifs}, {}};
  
  std::cout << "Day 02:" << '\n'
            << "  Part 1: " << std::ranges::count_if (data, &pass_info::part1) << '\n'
            << "  Part 2: " << std::ranges::count_if (data, &pass_info::part2) << '\n';
}
