

### 1. Dijkstra's Algorithm: Finding the Shortest Path

Imagine you're standing in a city and want to find the shortest walking time to every other location. Dijkstra's algorithm is a methodical way to do this.

**The Core Idea**: Start at a source point and explore outwards. Always prioritize exploring from the location you've reached that has the shortest total travel time from your start. It's a "greedy" approach because you always choose the path that seems best at the moment.

**The Key Components**:
* **Nodes (Vertices)**: These are the locations (e.g., intersections in a city).
* **Edges**: These are the paths connecting the locations (e.g., streets).
* **Weights**: This is the "cost" to travel an edge (e.g., the time it takes to walk down a street). For Dijkstra's to work, these weights **must not be negative**.

**The Steps**:
1.  **Initialization**:
    * Pick a starting node. The distance to this node is 0.
    * The distance to every other node is initially unknown, so we mark it as infinity.
    * Keep track of all nodes you haven't visited yet.

2.  **The Loop**:
    * From your set of unvisited nodes, pick the one with the smallest known distance from the start. Let's call this the `current_node`.
    * For the `current_node`, look at all its directly connected neighbors.
    * For each neighbor, calculate the distance from the start point *through* the `current_node`. This is `(distance to current_node) + (weight of the edge to the neighbor)`.
    * If this new calculated distance is shorter than the neighbor's previously known distance, update it.
    * Once you've checked all neighbors, mark the `current_node` as visited. You're done with it because you've found the guaranteed shortest path to it.
    * Repeat this process until you've visited all nodes (or just the destination node if you only need one path).



---

### 2. Priority Queues: The Engine for Dijkstra's

How do you efficiently perform the step "pick the unvisited node with the smallest known distance"? If you have thousands of nodes, searching through a list every time is very slow. This is where a priority queue comes in.

**What it is**: A priority queue is a special data structure where every element has a "priority." When you ask for an element, it **always gives you the one with the highest (or lowest) priority**.

**How it helps Dijkstra's**:
* You store the nodes to visit in a priority queue.
* The "priority" of each node is its **total distance from the start**.
* When Dijkstra's algorithm needs the next node to visit, it simply asks the priority queue for the element with the lowest priority (the shortest distance). This is extremely fast.

So, the priority queue acts as an efficient to-do list, ensuring the algorithm always explores the most promising node next without wasting time searching.

### APPLY IN AOC day 17
[[Day 17]]
