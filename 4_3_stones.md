
# Deep Dive: The Stone Removal Problem

## 1. The Core Intuition: Reversing Time
When we see a problem involving removing items from a sequence where the cost depends on the **adjacent elements**, a direct simulation or Greedy approach fails.
* Why? Because removing a stone right now changes the neighbors for the adjacent stones in the future. A seemingly "cheap" removal early on might isolate other stones, forcing you to pay exorbitant costs later.

Instead of thinking about which stone to remove **first**, we must reverse time and think about which stone is removed **last**. This strategy is famous in Competitive Programming (most notably the "Burst Balloons" DP pattern).

## 2. Why Interval DP?
Imagine a contiguous subarray of stones from index $i$ to $j$.
If we decide that a particular stone $k$ (where $i \le k \le j$) is the **absolute last stone to be removed** from this subarray:
1. All stones between $i$ and $k-1$ will be completely removed *before* $k$.
2. All stones between $k+1$ and $j$ will be completely removed *before* $k$.

Because $k$ is the last one standing in that interval, when it is finally time for $k$ to be removed, **who are its neighbors?**
Since everything strictly inside $[i, j]$ is already gone, $k$'s only possible neighbors are the stones sitting immediately **outside** the interval: stone $i-1$ and stone $j+1$.

This is the "Aha!" moment. The number of neighbors stone $k$ has when it is removed is entirely, deterministically bound by the boundaries of our current interval!
* If $i = 1$ and $j = N$, there are no stones outside. Stone $k$ has **0 neighbors**.
* If $i > 1$ and $j = N$, stone $k$ has **1 neighbor** (the one at $i-1$).
* If $i = 1$ and $j < N$, stone $k$ has **1 neighbor** (the one at $j+1$).
* If $i > 1$ and $j < N$, stone $k$ has **2 neighbors** ($i-1$ and $j+1$).

Because the state of the problem is perfectly defined by the bounding interval $[i, j]$, this implies a strict **Interval Dynamic Programming** approach.

## 3. Pattern Recognition for the Future
* **"Cost of removal depends on adjacent elements"**: The moment you see this phrase, your mind should instantly jump to **Interval DP**.
* **"Reverse the process"**: Whenever removing an element dynamically changes the array and makes future cost calculations messy, reverse your perspective. Pick the **last** element to be removed. This establishes stable boundaries.

## 4. Step-by-Step Algorithm Walkthrough
1. **State Definition:** Let `dp[i][j]` be the minimum cost to remove all stones strictly in the subsegment from index $i$ to index $j$.
2. **Boundary Condition:** If $i > j$, the cost is $0$ (represented by our DP table being initialized to $0$).
3. **Length Iteration:** We solve subproblems of increasing length, starting from `len = 1` up to `len = N`. This guarantees that when calculating `dp[i][j]`, the answers for smaller enclosed chunks like `dp[i][k-1]` are already computed.
4. **Neighbor Calculation:** For a given interval $[i, j]$, we check if $i > 1$ (the left boundary neighbor exists) and if $j < N$ (the right boundary neighbor exists). This tells us if $k$ will be removed with $0, 1,$ or $2$ neighbors.
5. **The Transition:** For the interval $[i, j]$, we iterate through every possible stone $k$ to be the **last** one removed.
   $$\text{Cost} = dp[i][k-1] + dp[k+1][j] + cost[k][outside\_neighbors]$$
   We take the minimum of this Cost over all possible choices of $k$.
6. **Final Answer:** After expanding our window up to size $N$, `dp[1][N]` will hold the absolute minimum cost to remove the entire
