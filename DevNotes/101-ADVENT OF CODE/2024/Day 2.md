
```python
import os

def is_safe_report(levels):
    """
    Checks if a list of levels meets the Part 1 safety criteria:
    1. Strictly monotonic (all increasing or all decreasing).
    2. All adjacent differences are in [1, 3].
    """
    if len(levels) < 2:
        return True # Trivial case: 0 or 1 element is always safe

    is_strictly_increasing = True
    is_strictly_decreasing = True
    
    for i in range(len(levels) - 1):
        diff = levels[i+1] - levels[i]
        abs_diff = abs(diff)
        
        # CRITERION 2: Check difference constraint (must be 1 to 3 for ALL pairs)
        if not (1 <= abs_diff <= 3):
            return False # Failed the difference test immediately
        
        # CRITERION 1: Update strict monotonicity flags
        if diff <= 0:
            is_strictly_increasing = False
        
        if diff >= 0:
            is_strictly_decreasing = False
    
    # Must be EITHER strictly increasing OR strictly decreasing (and passed diff test)
    return is_strictly_increasing or is_strictly_decreasing

# ----------------------------------------------------------------------
def is_part2_safe(report):
    """
    Checks if a report is safe under Part 2 rules:
    The original report OR any version with one element removed must be safe by Part 1 rules.
    """
    # 1. Check if the original report is already safe (covers Part 1 solution)
    if is_safe_report(report):
        return True

    # 2. Check every possible sub-report by removing one element
    for i in range(len(report)):
        # Create a new list by skipping the element at index i
        # report[:i] is the left part, report[i+1:] is the right part
        sub_report = report[:i] + report[i+1:]
        
        if is_safe_report(sub_report):
            return True # Found a safe sub-report, no need to check others

    return False # Neither the original nor any single-removal version was safe

def solve_part2():
    safe_reports_count = 0
    file_path = "input.txt" # Ensure your puzzle input is in 'input.txt'
    
    try:
        with open(file_path, 'r') as file:
            for line in file:
                cleaned_line = line.strip()
                if not cleaned_line:
                    continue

                try:
                    # Convert the line to a list of integers
                    levels = [int(x) for x in cleaned_line.split()]
                except ValueError:
                    print(f"Skipping line due to non-integer content: '{cleaned_line}'")
                    continue
                
                # Check the Part 2 safety rule
                if is_part2_safe(levels):
                    safe_reports_count += 1

    except FileNotFoundError:
        print(f"Error: Input file '{file_path}' not found.")
        return None
    
    return safe_reports_count

# --- Run the solution ---
final_answer = solve_part2()

if final_answer is not None:
    print(f"The number of Part 2 safe reports is: {final_answer}")

# --- Example Check (for verification) ---
# Example from Part 1 description (7 6 4 2 1 and 1 3 6 7 9 were safe)
# The Part 2 test requires the original report OR a single-removal report to be safe.
# Let's test the original unsafe examples to see if they become safe in Part 2.
print("\n--- Example Verification ---")
example_reports_part2 = [
    [7, 6, 4, 2, 1],  # Safe in Part 1 -> Safe in Part 2 (Expected: True)
    [1, 2, 7, 8, 9],  # Unsafe (diff 5). Removing 7 makes [1, 2, 8, 9] (diffs 1, 6, 1) -> Unsafe. Removing 2 makes [1, 7, 8, 9] (diffs 6, 1, 1) -> Unsafe. It might remain Unsafe.
    [9, 7, 6, 2, 1],  # Unsafe (diff 4). Removing 6 makes [9, 7, 2, 1] (diffs 2, 5, 1) -> Unsafe. Removing 2 makes [9, 7, 6, 1] (diffs 2, 1, 5) -> Unsafe. It might remain Unsafe.
    [1, 3, 2, 4, 5],  # Unsafe (not monotonic). Removing 3 makes [1, 2, 4, 5] (Safe). -> Safe in Part 2
    [8, 6, 4, 4, 1],  # Unsafe (diff 0). Removing one 4 makes [8, 6, 4, 1] (Safe). -> Safe in Part 2
    [1, 3, 6, 7, 9]   # Safe in Part 1 -> Safe in Part 2 (Expected: True)
]

# We don't have the final example count for Part 2, but let's count our checks.
example_part2_safe_count = sum(is_part2_safe(r) for r in example_reports_part2)

print(f"Example Part 2 safe count: {example_part2_safe_count} (Expected value depends on which sub-reports become safe)")

```