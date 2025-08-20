## Def:
Breadth-First Search (BFS) is ==a graph traversal algorithm that explores a graph level by level==

![[Pasted image 20250817174830.png]]
### ALGORITHM 
```python
visited = [False] * GRID_SIZE
def BFS(GRID, STARTING):
	queue = [STARTING]
	while(!queue.isEmpty()):
		NEW_NODE = queue.pop()
		if not visited:
			visit(NEW_NODE) #DO SOMETHING
			visited[NEW_NODE] = TRUE
			for w in GRID.NEIGHBORS(NEW_NODE):
				if !visited(w):
					queue.append(w)
```

### FLOOD FILL
- **PYTHON**
```python 
from collections import deque
class Solution:
    
    def floodFill(self, image: List[List[int]], sr: int, sc: int, color: int) -> List[List[int]]:
        start = image[sr][sc]
        visited = set()
        queue = deque([(sr, sc)])
        while(q):
            row,col = queue.popleft()
            visited.add(row,col)
            image[row][col] = color
            for row,col in neighbours(image,row,col,start):
                if(row,col) not in visited:
                    queue.append((row,col))
        return image
    def neighbours(image,row,col,start):
        indices = [(row-1,col),(row+1,col),(row,col-1),(row,col+1)]
        return [(row,col) for row,col in indices if isValid(image,row,col) and image[row,col] == start]
    def isValid(image,row,col):
        return row > 0 and col>0 and row<len(image) and col<len(image[0])


        
```