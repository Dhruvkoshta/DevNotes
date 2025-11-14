### Using recursion
```python
marksed = [False] * GRIDSIZE
def dfs(G,v):
	visit(v)
	marked[v] = True
	for w in neighbours(G,v):
		if not marked[w]:
			dfs(G,w)
```

### Using Stack
```python
marksed = [False] * GRIDSIZE
def dfs(G,v):
	stack = [v]
	while stackNotEmpty():
		v = stack.pop()
		if not marked[w]:
				visit(v)
				marked[v] = True
				for w in neighbours(G,v):
					if not marked[w]:
						stack.append(w)
```