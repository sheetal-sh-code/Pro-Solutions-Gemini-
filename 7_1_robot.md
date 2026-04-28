
# Deep Dive: The Robot Garbage Collection Problem

## 1. The Core Intuition: The "Wait Time" Penalty
At first glance, this looks like a simulation, but the penalty rule forces us into a mathematical approach.
The rule states: *"At any moment, the cost is the sum of remaining uncleaned garbage."*

Let's translate this into math. If we deploy a robot at index $L$, it cleans $A[L]$ immediately at time $t=0$. Then it moves to $L+1$ and cleans it at time $t=1$, and so on.
This means the garbage at index $i$ (where $i \ge L$) will sit uncleaned for exactly $(i - L)$ moments.

Therefore, the total cost added to our penalty by the garbage at index $i$ is exactly:
**$	ext{Cost}_i = A[i] 	imes (i - L)$**

Because leaving any garbage uncleaned forever results in an infinite cost, we must ensure every cell is covered. This means we are essentially **partitioning** the array into continuous subsegments. For each subsegment from $L$ to $R$, we deploy one robot at $L$, and the total cost for that segment is:
$$	ext{Segment Cost} = m + \sum_{k=L}^{R} A[k] 	imes (k - L)$$

## 2. Formulating the DP (The $O(N^2)$ Approach)
Let $DP[i]$ be the minimum cost to clean the prefix of the array up to index $i-1$.
To find $DP[i]$, we test all possible starting positions $j$ for the final robot:
$$DP[i] = \min_{0 \le j < i} \left( DP[j] + m + \sum_{k=j}^{i-1} A[k] 	imes (k - j)
ight)$$

We can optimize the summation using two prefix sum arrays:
1. $P[i] = \sum_{k=0}^{i} A[k]$
2. $S[i] = \sum_{k=0}^{i} k \cdot A[k]$

Using these, the cost simplifies perfectly to:
$$\sum_{k=j}^{i-1} k \cdot A[k] - j \sum_{k=j}^{i-1} A[k] = (S[i-1] - S[j-1]) - j \cdot (P[i-1] - P[j-1])$$

Substitute this back into the DP state:
$$DP[i] = m + S[i-1] + \min_{0 \le j < i} \left( DP[j] - S[j-1] + j \cdot P[j-1] - j \cdot P[i-1]
ight)$$

## 3. Convex Hull Trick (From $O(N^2)$ to $O(N)$)
If we look closely at the `min` function, it represents a linear equation $Y = M \cdot X + C$:
* **$X = P[i-1]$** (This represents our query point. Because $A[k] \ge 0$, $X$ is monotonically non-decreasing).
* **$M = -j$** (The slope. Since $j$ is strictly increasing, the slope is strictly decreasing).
* **$C = DP[j] - S[j-1] + j \cdot P[j-1]$** (The Y-intercept).

### Why the Convex Hull Trick?
Because our slopes are strictly decreasing ($-0, -1, -2 \dots$) and our queries ($X$) are strictly moving to the right, we are looking for the minimum of a set of intersecting lines.
We do not need a complex Segment Tree. We can maintain the "Lower Envelope" (the mathematical bottom edge) of these lines using a **Double-Ended Queue (Deque)**.

When a new line is formed, we check if it makes previous lines mathematically obsolete (they will never be the minimum again). If so, we pop them from the back. When we query for our answer, we check if the current best line has been overtaken by the next line in the queue. If so, we pop from the front.

This drops the time complexity from $\mathcal{O}(N^2)$ down to a blistering $\mathcal{O}(N)$.
