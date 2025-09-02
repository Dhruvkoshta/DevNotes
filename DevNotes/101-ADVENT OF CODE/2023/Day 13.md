### Pattern Matching and Symmetry Detection
- **%% Basically a two pointer problem %%

## The Approach
1. Parse the input into a grid.
2. Checking a Potential Reflection
	- Take 2 Pointers i and i+1 and check if same. 
	- if eq then check i-1 and i+2 till the end of line.
3. Continue for rows and columns both
4. Use transpose the grid to reuse the function.

```python
def find_horizontal_mirror(grid):

    for i in range(len(grid) - 1):
        top_ptr = i
        bottom_ptr = i + 1
        is_perfect_mirror = True

        while top_ptr >= 0 and bottom_ptr < len(grid):
            if grid[top_ptr] != grid[bottom_ptr]:
                is_perfect_mirror = False
                break
            
            top_ptr -= 1
            bottom_ptr += 1
            
        if is_perfect_mirror:
            return i + 1
            
    return 0

def solve(filename="input.txt"):

    try:
        with open(filename, 'r') as file:
            # Patterns are separated by blank lines.
            patterns = file.read().strip().split('\n\n')
    except FileNotFoundError:
        print(f"Error: Input file '{filename}' not found.")
        return

    total_summary = 0

    for pattern_str in patterns:
        # Convert each pattern block into a grid (list of strings).
        grid = pattern_str.splitlines()

        # 1. Check for a horizontal mirror.
        horizontal_rows = find_horizontal_mirror(grid)
        if horizontal_rows > 0:
            total_summary += 100 * horizontal_rows
            continue # Move to the next pattern once the mirror is found.

        # 2. If no horizontal mirror, check for a vertical one by transposing.
        transposed_grid = ["".join(col) for col in zip(*grid)]

        vertical_cols = find_horizontal_mirror(transposed_grid)
        if vertical_cols > 0:
            total_summary += vertical_cols

    return total_summary

if __name__ == "__main__":
    # You will need to create an 'input.txt' file with your puzzle input.
    result = solve()
    if result is not None:
        print(f"The final summary for Part 1 is: {result}")

```


# Part 2
```

```