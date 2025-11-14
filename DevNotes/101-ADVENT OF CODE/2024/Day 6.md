```cpp
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Represents the grid and guard's state
struct GuardSim {
  vector<string> grid;
  int startRow, startCol, startDir;
  int rows, cols;

  // Directions: 0: Up, 1: Right, 2: Down, 3: Left
  // Change in (row, col) for each direction
  const vector<pair<int, int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

  // Constructor to parse the input grid
  GuardSim(const vector<string> &inputGrid) {
    grid = inputGrid;
    rows = grid.size();
    cols = grid[0].size();
    findStart();
  }

  // Finds the guard's starting position and direction
  void findStart() {
    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        char ch = grid[r][c];
        if (ch == '^') {
          startRow = r;
          startCol = c;
          startDir = 0;
          return;
        }
        if (ch == '>') {
          startRow = r;
          startCol = c;
          startDir = 1;
          return;
        }
        if (ch == 'v') {
          startRow = r;
          startCol = c;
          startDir = 2;
          return;
        }
        if (ch == '<') {
          startRow = r;
          startCol = c;
          startDir = 3;
          return;
        }
      }
    }
  }

  // Checks if a position is within the grid boundaries
  bool isOutOfBounds(int r, int c) {
    return r < 0 || r >= rows || c < 0 || c >= cols;
  }

  /**
   * Simulates the guard's path.
   * detectsLoop:
   * - If true (for Part 2), returns 1 if a loop is found, 0 otherwise.
   * - If false (for Part 1), returns the count of visited tiles.
   */
  int simulate(bool detectsLoop) {
    int r = startRow;
    int c = startCol;
    int dir = startDir;

    // Set to store visited positions (row, col)
    set<pair<int, int>> visitedPos;

    // Set to store visited states (row, col, dir) for loop detection
    set<tuple<int, int, int>> visitedStates;

    // Safety break for very long loops, though state-based detection is primary
    for (int step = 0; step < 1000000; ++step) {

      if (detectsLoop) {
        // --- Part 2 Logic: Loop Detection ---
        tuple<int, int, int> state = {r, c, dir};
        if (visitedStates.count(state)) {
          return 1; // Loop detected!
        }
        visitedStates.insert(state);
      } else {
        // --- Part 1 Logic: Position Counting ---
        visitedPos.insert({r, c});
      }

      // 1. Calculate next position
      int nextR = r + moves[dir].first;
      int nextC = c + moves[dir].second;

      // 2. Check for exit
      if (isOutOfBounds(nextR, nextC)) {
        // Guard exits the grid
        return detectsLoop ? 0 : visitedPos.size();
      }

      // 3. Check for obstacle
      if (grid[nextR][nextC] == '#') {
        // Turn right
        dir = (dir + 1) % 4;
      } else {
        // Move forward
        r = nextR;
        c = nextC;
      }
    }

    // Should only be reached if loop detected or safety break hit
    return detectsLoop ? 1 : visitedPos.size();
  }
};

int main() {
  vector<string> inputGrid;
  string line;

  cout << "Paste your puzzle input and press Ctrl+D (Linux/macOS) or Ctrl+Z "
          "(Windows) to end:"
       << endl;
  while (getline(cin, line)) {
    if (!line.empty()) {
      inputGrid.push_back(line);
    }
  }

  if (inputGrid.empty()) {
    cerr << "No input provided!" << endl;
    return 1;
  }

  // --- Part 1 ---
  GuardSim simPart1(inputGrid);
  int part1_answer = simPart1.simulate(false);
  cout << "-----------------------------------" << endl;
  cout << "Part 1 Answer: " << part1_answer << endl;

  // --- Part 2 ---
  int loopFormingObstacles = 0;
  vector<pair<int, int>> emptyTiles;

  // Find all potential spots to add an obstacle
  for (int r = 0; r < simPart1.rows; ++r) {
    for (int c = 0; c < simPart1.cols; ++c) {
      if (simPart1.grid[r][c] == '.') {
        emptyTiles.push_back({r, c});
      }
    }
  }

  // Test each empty tile
  for (const auto &tile : emptyTiles) {
    // Create a copy of the grid and add the new obstacle
    vector<string> modifiedGrid = inputGrid;
    modifiedGrid[tile.first][tile.second] = '#';

    // Run simulation on the modified grid
    GuardSim simPart2(modifiedGrid);
    if (simPart2.simulate(true) == 1) {
      loopFormingObstacles++;
    }
  }

  cout << "Part 2 Answer: " << loopFormingObstacles << endl;
  cout << "-----------------------------------" << endl;

  return 0;
}

```