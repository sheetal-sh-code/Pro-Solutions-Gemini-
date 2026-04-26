# Deep Dive: Tree Re-rooting Dynamic Programming

## 1. The Core Intuition: Why Brute Force Fails
The problem asks us to find the best vertex $v$ to maximize the sum of $a_i 	imes d_i$.

* **The Naive Approach:** For every single node $v$, run a Breadth-First Search (BFS) or Depth-First Search (DFS) to find the distances to all other nodes, and calculate the sum.
* **The Problem:** Running a DFS takes $\mathcal{O}(N)$ time. Running it for *every* node takes $\mathcal{O}(N^2)$ time. For $N = 10^5$, this results in $10^{10}$ operations, which will instantly throw a **Time Limit Exceeded (TLE)** error.

[Image of Tree Rerooting technique showing shifting from parent to child]

## 2. The Paradigm Shift: Re-rooting
Imagine you have already calculated the complete answer for Node 1. You know the exact sum of $a_i 	imes d_i$ if Node 1 is the root. Let's call this `res[1]`.

Now, you decide to shift the root to Node 1's direct child, Node 2. Do you need to recalculate everything from scratch? **No!**
When you take one step from Node 1 to Node 2, what happens to all the distances?
* Every node that is *inside* Node 2's subtree just got **1 step closer** to you. Their distance decreases by 1.
* Every node that is *outside* Node 2's subtree (the rest of the tree) just got **1 step further away** from you. Their distance increases by 1.

## 3. The Magical Transition Formula
Let `sum_a[v]` be the sum of all $a_i$ values inside the subtree of $v$.
Let `total_a` be the sum of all $a_i$ values in the entire tree.

When moving the root from parent $u$ to child $v$:
1. We **lose** exactly `1 * sum_a[v]` from our total score because those nodes are now 1 step closer.
2. We **gain** exactly `1 * (total_a - sum_a[v])` to our total score because the rest of the nodes are now 1 step further away.

**The $\mathcal{O}(1)$ Formula:**
$$res[v] = res[u] - sum\_a[v] + (total\_a - sum\_a[v])$$

Using this formula, we can find the answer for a child node instantly if we know the answer for its parent.

## 4. Pattern Recognition for the Future
* **"Find a node in a tree that minimizes/maximizes distance sum"**: This is the absolute biggest red flag for Tree Re-rooting DP.
* **"Evaluate a global property for EVERY node as the root"**: Whenever you need to pretend every node is the root and calculate something, you cannot run $N$ DFS traversals. You run **two** DFS traversals. One bottom-up (to gather subtree data), and one top-down (to propagate the shifted answers).

## 5. Step-by-Step Algorithm Walkthrough
1. **Initialize & Pre-calculate:** Read the tree, and calculate `total_a`.
2. **First DFS (Post-order):** Pick an arbitrary root (e.g., Node 1). Run a standard DFS. As you climb back up the recursive calls, calculate `sum_a[u]` for every subtree. At the same time, calculate `res[1]`, which is the absolute baseline answer for Node 1.
3. **Second DFS (Pre-order):** Start at Node 1. For every child $v$, calculate `res[v]` using our $\mathcal{O}(1)$ Transition Formula. Then, recursively call the DFS on $v$ to pass this new answer down to its children.
4. **Find the Maximum:** Iterate through the `res` array from $1$ to $N$ and print the highest value!
