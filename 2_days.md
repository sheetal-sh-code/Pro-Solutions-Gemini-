# Algorithm Explanation: Minimum Days to Clear Warehouse Stock

## 1. The Core Intuition: Why "Savings" instead of "Simulation"?
When you first read the problem, the natural instinct is to simulate it day by day: add $B$ to everything, greedily pick the largest item to set to $0$, and check the sum.

However, we must abandon simulation. **Why?** * The item you choose to clear on Day 1 might get massive inflows on Day 2 and Day 3, making it a terrible choice in hindsight.
* The total sum is **not strictly decreasing** day by day! Depending on the inflows, the total stock might actually go *up* before it goes down.

Because the total stock isn't strictly decreasing, **Binary Search on the answer (days) will also fail.** We cannot simply say "If day 5 works, day 6 must work."

**The "Time Travel" Perspective:**
Instead of moving forward, let's fix a timeline of $m$ days and work backwards.
* If an item is **never** cleared, its final stock after $m$ days is $A_i + m \cdot B_i$.
* If an item is cleared on the very last day (Day $m$), its final stock is exactly $0$.
* If an item is cleared on Day $m-1$, it gathers inflow on Day $m$, so its final stock is $1 \cdot B_i$.
* If an item is cleared on Day $m-k$, its final stock is $k \cdot B_i$.

Instead of calculating the messy final stock directly, we calculate our **Savings**.
If we decide to clear item $x$, we replace its massive $A_x + m \cdot B_x$ stock with a much smaller $k \cdot B_x$.
The reduction (savings) is:
$$Savings = (A_x + m \cdot B_x) - (k \cdot B_x) = A_x + (m - k) \cdot B_x$$

Notice the multiplier $(m-k)$. If we clear exactly $m$ items over $m$ days, the multipliers applied to their $B_i$ values will be exactly $1, 2, 3, \dots, m$.

## 2. Why pick Dynamic Programming (DP)?
We want to maximize our savings. To do this, we need to pair the largest multipliers ($m, m-1, \dots, 1$) with the items that have the largest daily inflows ($B_i$).

1. **Why Sort?** We sort all items in **ascending** order of their $B_i$. This guarantees that if we pick a subsequence of items, the ones picked later automatically get the higher multipliers, maximizing our savings.
2. **Why DP?** We cannot just greedily pick the top $m$ items with the highest $B_i$, because their $A_i$ values might be tiny. We need to evaluate the combined weight of $A_i$ and the multiplier on $B_i$. Dynamic Programming is the perfect tool to evaluate "Pick vs. Skip" for every item while maintaining the order of multipliers.

## 3. The DP State Breakdown
* **State Definition:** $DP[j]$ = Maximum savings achieved by picking exactly $j$ items out of the ones we have processed so far.
* **Transitions:** For each item, we iterate backward from $j = i+1$ down to $1$.
   * If we pick this item to be the $j$-th cleared item, our savings increase by $A_i + j \cdot B_i$.
   * We update: $DP[j] = \max(DP[j], DP[j-1] + A_i + j \cdot B_i)$.
* **Why 1D Array (Space Optimized)?** We loop $j$ backwards (`j = i+1 down to 1`). If we looped forward, updating $DP[j]$ would affect $DP[j+1]$ in the exact same step, meaning we could accidentally clear the same item multiple times. Looping backward ensures each item is only considered once per timeline, allowing us to drop the $N \times N$ 2D matrix entirely to save memory.
