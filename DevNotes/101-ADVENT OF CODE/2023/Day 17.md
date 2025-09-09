
### The Problem: Advent of Code 2023, Day 17

**The Goal**: Find the path from the top-left corner to the bottom-right corner with the minimum total heat loss.

**The Twist (The Constraints)**: This isn't a simple pathfinding problem. Your movement is restricted:
* You cannot reverse direction (e.g., if you moved right, your next move can't be left).
* **Crucible Constraint (Part 1)**: You can move at most **three** steps in a single direction before you **must** turn 90 degrees left or right.

This constraint changes everything. The shortest path to a grid cell is meaningless if you arrive there having already moved three steps in the same direction, as your options for the next move are now limited.


---

###  The Solution: Combining Dijkstra's with State

To solve this, we must adapt Dijkstra's algorithm. The key is to realize that a "node" in our graph isn't just a position `(row, col)`. It has to include the information needed to follow the rules.

#### Step 1: Redefine the "Node" as a "State"

A state must uniquely identify a point in our search and contain enough information to determine the next valid moves. A good state representation is a tuple:

`state = (row, col, direction, consecutive_steps)`

* `row, col`: Your current grid coordinates.
* `direction`: The direction you entered this cell from (e.g., North, East, South, West).
* `consecutive_steps`: How many steps in a row you've taken in that `direction`.

#### Step 2: Adapt Dijkstra's with the New State

We will use a priority queue, but instead of storing `(distance, row, col)`, we'll store the full state information, prioritized by heat loss:

`Priority Queue Item: (total_heat_loss, row, col, direction, consecutive_steps)`

We also need a `visited` set to avoid processing the exact same *state* twice. Notice this is a set of states, not just coordinates. `visited.add((row, col, direction, steps))`

#### Step 3: Detailed Walkthrough of the Algorithm

1.  **Initialization**:
    * Create an empty priority queue.
    * Create an empty `visited` set.
    * The starting point is `(0, 0)`. You can start by moving Right or Down. So, push two initial states onto the queue. (Note: The puzzle is about the heat loss of the *entered* cell, so the starting cell `(0,0)` has 0 cost).
        * `pq.push((grid[0][1], 0, 1, EAST, 1))`
        * `pq.push((grid[1][0], 1, 0, SOUTH, 1))`

2.  **The Main Loop**:
    * **Pop the state with the lowest heat loss**. Let's say we pop `(10, 3, 4, NORTH, 2)`. This means we've reached cell `(3, 4)` with a total heat loss of 10, having come from the south (moved North) for 2 consecutive steps.
    * **Check if visited**. If `(3, 4, NORTH, 2)` is in our `visited` set, we've already found a better or equal path to this exact state. `continue` to the next item in the queue. Otherwise, add it to `visited`.
    * **Check for Goal**. Is `(row, col)` the bottom-right corner? If yes, we're done! The `total_heat_loss` is our answer.
    * **Generate Next Moves (Neighbors)**. From our current state `(row=3, col=4, dir=NORTH, steps=2)`, what can we do next?
        * **Turn Left (move West)**: This is a valid turn. The new state would be at `(3, 3)`. The number of steps resets to 1.
            * New state: `(row=3, col=3, dir=WEST, steps=1)`
            * New heat loss: `10 + grid[3][3]`
            * Push `(10 + grid[3][3], 3, 3, WEST, 1)` to the queue.
        * **Turn Right (move East)**: This is also a valid turn. The new state would be at `(3, 5)`. Steps reset to 1.
            * New state: `(row=3, col=5, dir=EAST, steps=1)`
            * New heat loss: `10 + grid[3][5]`
            * Push `(10 + grid[3][5], 3, 5, EAST, 1)` to the queue.
        * **Go Straight (move North)**: Can we continue North? Yes, because our current `steps=2` is less than the max of 3.
            * New state: `(row=2, col=4, dir=NORTH, steps=3)`
            * New heat loss: `10 + grid[2][4]`
            * Push `(10 + grid[2][4], 2, 4, NORTH, 3)` to the queue.

    * **Repeat**: The loop continues, always processing the state with the absolute minimum heat loss found so far across all possible paths, until the destination is reached.

## pseudo code


```
// Define constants for directions for clarity
// For example: 0:North, 1:East, 2:South, 3:West
DIRECTIONS = [( -1, 0 ), ( 0, 1 ), ( 1, 0 ), ( 0, -1 )]

function solve(grid):
    // The priority queue stores tuples of:
    // (total_heat_loss, row, col, direction_index, consecutive_steps)
    priority_queue = new PriorityQueue()

    // The visited set stores tuples of:
    // (row, col, direction_index, consecutive_steps)
    // to avoid redundant computations for the exact same state.
    visited = new Set()

    // Get grid dimensions
    height = number of rows in grid
    width = number of columns in grid

    // -- Initialization --
    // We start at (0,0) but haven't moved yet. We can either go
    // East (1) or South (2) as our first move.
    // The starting cell's heat loss is not counted.
    // So, we push the states for the first move onto the queue.
    
    // Move East from start
    if (0, 1) is a valid coordinate:
        heat_loss = grid[0][1]
        priority_queue.push((heat_loss, 0, 1, 1, 1)) // dir=East, steps=1

    // Move South from start
    if (1, 0) is a valid coordinate:
        heat_loss = grid[1][0]
        priority_queue.push((heat_loss, 1, 0, 2, 1)) // dir=South, steps=1

    // -- Main Dijkstra Loop --
    while priority_queue is not empty:
        // 1. Pop the state with the minimum heat loss
        (heat_loss, r, c, dir, steps) = priority_queue.pop()

        // 2. Check if we reached the destination
        if r == height - 1 and c == width - 1:
            return heat_loss // We found the shortest path!

        // 3. Check if we have already processed this exact state
        if (r, c, dir, steps) is in visited:
            continue
        visited.add((r, c, dir, steps))

        // 4. Explore next possible moves (neighbors in the state graph)

        // -- A) Continue Straight --
        // Check if we can continue in the same direction (steps < 3)
        if steps < 3:
            // Calculate next position
            nr = r + DIRECTIONS[ddir][0]
            nc = c + DIRECTIONS[dir][1]

            // Check if the new position is within grid bounds
            if 0 <= nr < height and 0 <= nc < width:
                new_heat_loss = heat_loss + grid[nr][nc]
                new_state = (new_heat_loss, nr, nc, dir, steps + 1)
                priority_queue.push(new_state)

        // -- B) Turn Left & Right --
        // We can always turn. We iterate through all four directions.
        for new_dir in range(4):
            // We cannot go in the same direction (handled above) or reverse.
            // The reverse of dir `d` is `(d + 2) % 4`.
            if new_dir == dir or new_dir == (dir + 2) % 4:
                continue

            // Calculate next position based on the new direction
            nr = r + DIRECTIONS[new_dir][0]
            nc = c + DIRECTIONS[new_dir][1]
            
            // Check if the new position is within grid bounds
            if 0 <= nr < height and 0 <= nc < width:
                new_heat_loss = heat_loss + grid[nr][nc]
                // After a turn, consecutive steps reset to 1
                new_state = (new_heat_loss, nr, nc, new_dir, 1)
                priority_queue.push(new_state)

    return -1 // Should not be reached if a path exists
```