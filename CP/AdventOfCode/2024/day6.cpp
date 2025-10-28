#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::ifstream file("input.txt");
  std::string content;
  char ch;
  while (file.get(ch)) {
    content += ch;
  }

  std::cout << content << std::endl;
  std::vector<char> GRID;
  for (int i = 0; i < content.size(); i++) {
    GRID.push_back(content[i]);
  }
}
