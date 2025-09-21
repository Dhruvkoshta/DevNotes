#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
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
  coordinates.push_back({0, 0}); // Start at origin
  std::pair<long, long> pr = {0, 0};
  long long perimeter = 0;

  while (getline(content, line)) {
    if (line.length() < 3)
      continue;

    // --- Part 2 Parsing Logic ---
    size_t hex_start = line.find('#') + 1;
    std::string hex_code = line.substr(hex_start, 6);

    // Extract distance (first 5 hex digits)
    long s = std::stol(hex_code.substr(0, 5), nullptr, 16);

    // Extract direction (last hex digit)
    char d = hex_code.back();

    perimeter += s;

    switch (d) {
    case '0': // R
      pr.first += s;
      break;
    case '1': // D
      pr.second += s;
      break;
    case '2': // L
      pr.first -= s;
      break;
    case '3': // U
      pr.second -= s;
      break;
    }
    coordinates.push_back(pr);
  }

  content.close();

  // Shoelace formula calculates the interior area
  long long shoelace_area = areaCalc(coordinates);

  // Pick's Theorem to find the total area, including the trench
  long long total_area = shoelace_area + (perimeter / 2) + 1;

  std::cout << "Total Dig Site Area: " << total_area << std::endl;

  return 0;
}
