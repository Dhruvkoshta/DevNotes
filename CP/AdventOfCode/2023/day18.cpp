#include <cmath>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

// Shoelace formula to calculate the interior area of a polygon
long long areaCalc(const std::vector<std::pair<long, long>> &myvec) {
  long long area = 0;
  size_t num_items = myvec.size();

  for (size_t i = 0; i < num_items; ++i) {
    long long x1 = myvec[i].first;
    long long y1 = myvec[i].second;
    long long x2 = myvec[(i + 1) % num_items].first;
    long long y2 = myvec[(i + 1) % num_items].second;

    area += (x1 * y2) - (x2 * y1);
  }
  return std::abs(area) / 2;
}

int main() {
  std::ifstream content{"input.txt"};
  if (!content.is_open()) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return 1;
  }

  std::string line;
  std::vector<std::pair<long, long>> coordinates;
  std::pair<long, long> pr = {0, 0};
  long long perimeter = 0;

  while (getline(content, line)) {
    if (line.length() < 3)
      continue;

    char d = line[0];
    long s = std::stoi(line.substr(2, line.find(' ', 2) - 2));

    perimeter += s;

    switch (d) {
    case 'R':
      pr.first += s;
      break;
    case 'D':
      pr.second += s;
      break;
    case 'U':
      pr.second -= s;
      break;
    case 'L':
      pr.first -= s;
      break;
    }
    coordinates.push_back(pr);
  }

  content.close();

  // Shoelace formula calculates the interior area (I + B/2 - 1)
  long long shoelace_area = areaCalc(coordinates);

  // Pick's Theorem to find the total area, including the trench
  long long total_area = shoelace_area + (perimeter / 2) + 1;

  std::cout << "Shoelace Area (Interior): " << shoelace_area << std::endl;
  std::cout << "Perimeter: " << perimeter << std::endl;
  std::cout << "Total Dig Site Area: " << total_area << std::endl;

  return 0;
}
