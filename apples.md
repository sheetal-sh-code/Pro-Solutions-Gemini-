Deep Dive: Apple Game - Right Turn Only

## 1. The Core Intuition: Why Greedy or Dijkstra Fails
A naive approach would be to find the shortest path from `Apple 1` to `Apple 2`, then `Apple 2` to `Apple 3` using standard pathfinding.

**The Flaw:** When you arrive at `Apple 1`, you might be facing South, requiring $2$ right turns to head toward `Apple 2`. But what if you took a slightly longer physical path to arrive at `Apple 1` facing East, which requires $0$ right turns to get to `Apple 2`?

Because your *arrival orientation* dictates your *future cost*, you cannot separate the apples into isolated sub-problems. You must track the global state across the entire sequence of apples.

## 2. Defining the State Space
To track everything accurately, we bundle our current reality into 4 dimensions:
* **`a`**: The number of apples eaten so far ($0$ to $M$).
* **`r`, `c`**: Our current row and column on the grid.
* **`d`**: Our current facing direction ($0$ to $3$).

By multiplying the constraints ($151 \text{ apples} \times 60 \text{ rows} \times 60 \text{ cols} \times 4 \text{ directions}$), we find there are only about $2.17 \times 10^6$ total possible states. This is incredibly tiny for modern CPUs. We flatten this 4D state into a 1D Array to achieve maximum memory cache efficiency and speed.

## 3. Why 0-1 BFS?
We have exactly two available actions at any point:
1. **Move Forward**: Steps into the next cell. This costs **0** right turns.
2. **Turn Right**: Changes our direction `(d + 1) % 4`. This costs **1** right turn.

Because our graph edge weights are strictly `0` or `1`, standard BFS fails (it requires all edges to be identical, like `1`), and Dijkstra's Algorithm is overkill (the priority queue adds unnecessary $O(\log V)$ time overhead).

Instead, we use a **Double-Ended Queue (Deque)**:
* When an action costs `0`, we bypass the line and push the new state to the **front** of the queue.
* When an action costs `1`, we push it to the **back** of the queue.

This gracefully guarantees that we are always exploring paths with the absolute minimum number of right turns first, perfectly mimicking Dijkstra but in pure $\mathcal{O}(V + E)$ linear time.

## 4. Pattern Recognition for the Future
* **"Cost is only applied to turning / changing state, not moving"**: This is the absolute biggest red flag for a **0-1 BFS** on a grid.
* **"Must visit targets in a strict order"**: Whenever sequence matters, add the `target_index` as an extra dimension to your BFS state. It seamlessly integrates the sequence into the pathfinding.
