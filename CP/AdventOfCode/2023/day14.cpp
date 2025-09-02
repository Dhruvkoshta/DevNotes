
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
void tilt_north(std::vector<std::vector<char>> &input) {
  for (size_t j = 0; j < input[0].size(); ++j) {
    size_t empty_row = 0;
    for (size_t i = 0; i < input.size(); ++i) {
      if (input[i][j] == '#') {
        empty_row = i + 1;
      } else if (input[i][j] == 'O') {
        if (i != empty_row) {
          input[empty_row][j] = 'O';
          input[i][j] = '.';
        }
        empty_row++;
      }
    }
  }
}

void tilt_west(std::vector<std::vector<char>> &input) {
  for (size_t i = 0; i < input.size(); ++i) {
    size_t empty_col = 0;
    for (size_t j = 0; j < input[i].size(); ++j) {
      if (input[i][j] == '#') {
        empty_col = j + 1;
      } else if (input[i][j] == 'O') {
        if (j != empty_col) {
          input[i][empty_col] = 'O';
          input[i][j] = '.';
        }
        empty_col++;
      }
    }
  }
}

void tilt_south(std::vector<std::vector<char>> &input) {
  for (size_t j = 0; j < input[0].size(); ++j) {
    size_t empty_row = input.size() - 1;
    for (int i = input.size() - 1; i >= 0; --i) {
      if (input[i][j] == '#') {
        empty_row = i - 1;
      } else if (input[i][j] == 'O') {
        if (static_cast<size_t>(i) != empty_row) {
          input[empty_row][j] = 'O';
          input[i][j] = '.';
        }
        empty_row--;
      }
    }
  }
}

void tilt_east(std::vector<std::vector<char>> &input) {
  for (size_t i = 0; i < input.size(); ++i) {
    size_t empty_col = input[i].size() - 1;
    for (int j = input[i].size() - 1; j >= 0; --j) {
      if (input[i][j] == '#') {
        empty_col = j - 1;
      } else if (input[i][j] == 'O') {
        if (static_cast<size_t>(j) != empty_col) {
          input[i][empty_col] = 'O';
          input[i][j] = '.';
        }
        empty_col--;
      }
    }
  }
}

long calculate_load(const std::vector<std::vector<char>> &input) {
  long total_load = 0;
  int size = input.size();
  for (int i = 0; i < size; ++i) {
    long count = 0;
    for (char ch : input[i]) {
      if (ch == 'O') {
        count++;
      }
    }
    total_load += count * (size - i);
  }
  return total_load;
}
std::string grid_to_string(const std::vector<std::vector<char>> &grid) {
  std::string s = "";
  for (const auto &row : grid) {
    s.append(row.begin(), row.end());
  }
  return s;
}
int main() {
  std::ifstream infile{"input.txt"};
  // ... (file reading code) ...
  if (!infile.is_open()) {
    std::cerr << "Error: Could not open file." << std::endl;
    return 1;
  }

  std::string line;
  std::vector<std::vector<char>> input;
  while (std::getline(infile, line)) {
    input.push_back(std::vector<char>(line.begin(), line.end()));
  }

  std::map<std::string, int> seen_grids;
  std::vector<long> loads;
  int cycle_start = 0;
  int cycle_len = 0;

  const long long total_cycles = 1000000000LL;

  for (long long i = 0; i < total_cycles; ++i) {
    // One complete spin cycle (N, W, S, E)
    tilt_north(input);
    tilt_west(input);
    tilt_south(input);
    tilt_east(input);

    // Convert the grid to a string to use as a map key
    std::string current_grid_state = grid_to_string(input);

    // Check if we have seen this state before
    if (seen_grids.count(current_grid_state)) {
      cycle_start = seen_grids[current_grid_state];
      cycle_len = i - cycle_start;

      // Calculate the remaining cycles and the offset into the cycle
      long long remaining_cycles = total_cycles - 1 - i;
      int offset = remaining_cycles % cycle_len;

      // Output the load corresponding to the final state
      std::cout << loads[cycle_start + offset] << '\n';
      return 0;
    }

    // If not seen, store the state and the current load
    seen_grids[current_grid_state] = i;
    loads.push_back(calculate_load(input));
  }

  // This part is for the rare case where no cycle is found within the loop
  std::cout << calculate_load(input) << '\n';

  return 0;
}
