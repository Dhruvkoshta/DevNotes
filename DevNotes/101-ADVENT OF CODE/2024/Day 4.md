```python
grid = []
with open("input.txt", 'r') as f:
   grid =  [line.strip() for line in f if line.strip()]

R = len(grid)
C = len(grid[0])

def isInside(r,c):
    return 0 <= r < R and 0 <= c < C

directions = [(0,1),(1,0),(0,-1),(-1,0),(1,1),(1,-1),(-1,1),(-1,-1)]

word = "XMAS"
count = 0

for r in range(R):
    for c in range(C):
        if grid[r][c] == 'X':

            for dir_r, dir_c in directions:
                match_x = True
                for step in range(1,4):
                    nr,nc = r + dir_r * step, c + dir_c * step
                    if not isInside(nr,nc) or grid[nr][nc] != word[step]:
                        match_x = False
                        break
                if match_x:
                    count += 1

def solve_part2(grid):
    R = len(grid)
    C = len(grid[0])
    count = 0

    # The 4 diagonal offset coordinates (dr, dc)
    DIAGONALS = [(-1, -1), (-1, 1), (1, -1), (1, 1)] # Top-L, Top-R, Bottom-L, Bottom-R

    for r in range(1, R - 1): # 'A' must be at least one step from the edge
        for c in range(1, C - 1):
            
            if grid[r][c] == 'A':
                # The 4 surrounding diagonal characters
                chars = []
                for dr, dc in DIAGONALS:
                    chars.append(grid[r + dr][c + dc])

                # The four diagonal characters are at indices 0, 1, 2, 3
                # chars[0] = Top-Left (TL)
                # chars[1] = Top-Right (TR)
                # chars[2] = Bottom-Left (BL)
                # chars[3] = Bottom-Right (BR)
                
                # We need exactly two 'M's and two 'S's
                if sorted(chars) == ['M', 'M', 'S', 'S']:
                    
                    # We only care about the diagonals pairing up:
                    # 1. TL-BR and TR-BL (M.A.S on one diagonal, S.A.M on the other)
                    # OR
                    # 2. TL-TR and BL-BR (M's on top row, S's on bottom, or vice-versa)
                    
                    # Combination 1: Diagonal MAS/SAM
                    # e.g., M-S and S-M pairings
                    if (chars[0] != chars[3]) and (chars[1] != chars[2]):
                        count += 1
                        
                    # Combination 2: Horizontal M/S rows
                    # e.g., M-M and S-S pairings
                    elif (chars[0] == chars[1]) and (chars[2] == chars[3]):
                        count += 1
                        
    return count
               
print(solve_part2(grid))

```