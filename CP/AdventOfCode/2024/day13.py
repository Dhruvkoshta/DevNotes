import re

def solve_2x2_system(a1, b1, c1, a2, b2, c2):

    D = a1 * b2 - a2 * b1

    if D == 0:
        # Check for consistency (no solution or infinite solutions)
        Dx = c1 * b2 - c2 * b1
        Dy = a1 * c2 - a2 * c1
        if Dx == 0 and Dy == 0:
            return "The system has infinite solutions (lines are dependent)."
        else:
            return "The system has no solution (lines are parallel)."

    # Calculate the determinants for 'a' (Da) and 'b' (Db)
    # Da (for the first variable, 'a') = c1*b2 - c2*b1
    Da = c1 * b2 - c2 * b1
    a = Da / D

    # Db (for the second variable, 'b') = a1*c2 - a2*c1
    Db = a1 * c2 - a2 * c1
    b = Db / D
    
    return a, b
file  = open("input.txt")
batch = []
total_tokens = 0
for line in file.readlines():

    line = line.strip()
    pattern = r"X\+(\d+), Y\+(\d+)"
    pattern2 = r"X=(\d+), Y=(\d+)"

    match = re.search(pattern, line)
    match2 = re.search(pattern2, line)
    x_val, y_val = 0, 0
    if match:
        x_val = int(match.group(1))
        y_val = int(match.group(2))
    if match2:
        x_val = int(match2.group(1))
        y_val = int(match2.group(2))
    if x_val != 0 and y_val != 0:
        batch.append((x_val, y_val))
for i in range(0,len(batch),3):
    a1,b1 = batch[i]
    a2,b2 = batch[i+1]
    x,y = batch[i+2]
    x = x + 10000000000000
    y = y + 10000000000000
    result_a, result_b = solve_2x2_system(a1, a2, x, b1, b2, y)

    if result_a is not None and result_b is not None:
        if result_a % 1 == 0 and result_b % 1 == 0:
            token = int(result_a) * 3 + int(result_b) * 1
            total_tokens = total_tokens + token
    
print(total_tokens)
