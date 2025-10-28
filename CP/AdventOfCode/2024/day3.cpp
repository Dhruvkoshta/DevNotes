#include <fstream>
#include <iostream>
#include <regex>
#include <string>

int main() {
  std::ifstream file("input.txt");
  std::string content;
  char ch;
  while (file.get(ch)) {
    content += ch;
  }

  std::regex mul_pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
  std::smatch match;
  std::string::const_iterator searchStart(content.cbegin());
  int part1_sum = 0;

  while (regex_search(searchStart, content.cend(), match, mul_pattern)) {
    int x = stoi(match[1]);
    int y = stoi(match[2]);
    part1_sum += x * y;
    searchStart = match.suffix().first;
  }
  std::cout << "Part 1: " << part1_sum << std::endl;

  std::regex full_pattern(R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))");
  searchStart = content.cbegin();
  int part2_sum = 0;
  bool enabled = true;

  while (regex_search(searchStart, content.cend(), match, full_pattern)) {
    std::string matched = match[0];

    if (matched == "do()") {
      enabled = true;
    } else if (matched == "don't()") {
      enabled = false;
    } else if (enabled) {
      int x = stoi(match[1]);
      int y = stoi(match[2]);
      part2_sum += x * y;
    }

    searchStart = match.suffix().first;
  }

  std::cout << "Part 2: " << part2_sum << std::endl;
}
