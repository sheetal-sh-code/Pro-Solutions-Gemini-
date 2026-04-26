# Deep Dive: Balancing the Military Tree

## 1. The Core Intuition: Why Greedy Fails
When asked to equalize children's subtree sums, the immediate instinct is to just find the maximum achievable sum for each child and force them all down to the minimum of those maximums.

**Why this fails:** Because a child's sum can skip values! If a child node has 2 children itself, its achievable sums might only be EVEN numbers. If you try to force it to an ODD number to match its siblings, it becomes impossible, and the whole tree balance breaks.

We must know **all** exact achievable sums for every single node.

## 2. State Compression (Bitset DP)
Look at the constraints: Max nodes = 500, Max soldiers per node = 100.
This guarantees the maximum possible total sum of the entire tree is strictly bounded by 50,000.

Instead of passing arrays or vectors of achievable sums up the tree, we can represent a node's achievable sums as a boolean array of size 50,005. In C++, `std::bitset` compresses this boolean array down to the bit-level, allowing us to perform operations on 64 sums simultaneously in a single CPU cycle.

## 3. Pattern Recognition for the Future
* **"All children must have the same value"**: This phrase dictates an **Intersection** operation. If node A can do {2, 4, 5} and node B can do {4, 5, 8}, the parent must pick from the intersection: {4, 5}.
* **$N \le 500$ and $val \le 100$**: Tiny combinations of constraints that yield a maximum possible state around $10^4$ to $10^5$ are neon signs pointing to `std::bitset` Knapsack-style DP.

## 4. Step-by-Step Algorithm Walkthrough
1. **Leaf Nodes:** A leaf node has no children to balance. Therefore, it can just delete its own soldiers freely. If it has 5 soldiers, it can achieve any sum in the set `{0, 1, 2, 3, 4, 5}`.
2. **Finding Common Ground (`common &= dp[v]`):** For an internal node, we recursively calculate the bitsets for all its children. We then use the Bitwise AND operator (`&`) to instantly find the intersection of all their achievable sums. Let's call a valid intersection sum `c`.
3. **Calculating the Base Contribution:** If the parent has $k$ children, and they all agree to balance themselves to sum `c`, the total sum contributed by the children is $k \times c$. We build a temporary bitset called `valid_bases` where we set the bit at index $k \times c$ to true.
4. **Adding the Parent's Soldiers:** The parent node itself currently holds `val[u]` soldiers. It can keep anywhere from $0$ to `val[u]` soldiers. To find the final achievable sums for the parent, we take our `valid_bases` bitset and shift it left using the `<<` operator for every possible amount of kept soldiers.
   *(E.g., if a valid base from the children is 10, and the parent can keep 0, 1, or 2 soldiers, the parent's achievable sums become 10, 11, and 12).*
5. **The Final Answer:** Once the DFS completes and returns to the root node, we just scan the root's bitset from right to left (50000 down to 0). The first bit we see set to `true` is the absolute maximum balanced sum for the entire tree!

## Complexity
* **Time Complexity:** Intersecting the children takes $O(N \times \frac{MAX\_SUM}{64})$. Shifting the valid bases takes $O(val \times \frac{MAX\_SUM}{64})$. Over all $N$ nodes, this runs roughly in $\approx 4 \times 10^7$ bitwise operations. This executes in a fraction of a millisecond, completely destroying the time limit.
* **Space Complexity:** $O(N \times MAX\_SUM)$ bits. This requires barely 3.1 Megabytes of RAM.
