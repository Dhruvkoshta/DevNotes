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
  std::vector<std::string> GRID;

  for (int i = 0; i < content.size(); i++) {
    for (int j = 0; j < content.size(); j++) {
      GRID.push_back(content[i][j]);
    }
  }
}
